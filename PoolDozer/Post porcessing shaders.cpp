#include <string>

#include <vector>
#include <chrono>


class Timer
{
public:
    Timer() : m_startTime(clock::now()) {}
    void Reset() { m_startTime = clock::now(); }
    int64_t ElapsedAsSeconds() const {
        return std::chrono::duration_cast<std::chrono::seconds> (clock::now() - m_startTime).count();
    }
    int64_t ElapsedAsMiliseconds() const {
        return std::chrono::duration_cast<std::chrono::milliseconds> (clock::now() - m_startTime).count();
    }

private:
    typedef std::chrono::high_resolution_clock clock;
    std::chrono::time_point<clock> m_startTime;
};

class Reductor{
public:
    bool Create();
    void Reduce(GLuint VAO, GLuint inFBO, GLuint sourceTex,GLuint width, GLuint height, GLuint fillProgram, GLuint RedProgram, GLuint swizzleProgram, bool isColor);
    void SwapBuffer();

    GLuint m_FBOs[12];
    GLuint m_currentBuffer;
    GLuint m_sampler;
    GLuint m_textures[12];
    GLint m_sizes[12] = {1,2,4,8,16,32,64,128,256,512,1024,2048};
};

bool Reductor::Create() {
    //initialize textures
    glGenTextures(12, m_textures);
    glGenFramebuffers(12, m_FBOs);
    glGenSamplers(1, &m_sampler);
    bool statusCheck[12];
    m_currentBuffer = m_FBOs[0];

    for(int index = 0; index < 12; ++ index)
    {
        glBindTexture(GL_TEXTURE_2D, m_textures[index]);
        glTexImage2D(GL_TEXTURE_2D,0, GL_RGB8, m_sizes[index], m_sizes[index], 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    bool status;
    for(int index = 0; index < 12; ++index)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_FBOs[index]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_textures[index],0);
        status = glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
        statusCheck[index] = status;
    }
    status = true;
    int index = 0;
    while (status == true && index < 12)
    {
       status = statusCheck[index];
        ++index;
    }

    glBindFramebuffer(GL_FRAMEBUFFER,0);

    //setup sampler
    glSamplerParameteri(m_sampler, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glSamplerParameteri(m_sampler, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glSamplerParameteri(m_sampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glSamplerParameteri(m_sampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    return status;
}

//reduction results are stored in m_textures[0] 1 x 1 texture
void Reductor::Reduce(GLuint VAO, GLuint inFBO, GLuint sourceTex, GLuint width, GLuint height,
                        GLuint fillProgram, GLuint RedProgram, GLuint swizzleProgram,bool isColor) {

    //1. Fill 2048 x 2048 RGB texture with one luminance value from camera frame, possibly bound to framebuffer --> single color RGB texture
    //2. Use glCopyTexImage2D and copy camera frame texture bound to FBO to 2048 x 2048 RGB texture --> square texture with excesive pixel set to single color from step 1.
    //3. Run reduction
    //use program that copies single value to whole texture
    int index = 11;
    glViewport(0,0, m_sizes[index], m_sizes[index]);
    glUseProgram(fillProgram);
    GLuint samplerParam = glGetUniformLocation(fillProgram, "u_Sampler");
    glUniform1i(samplerParam, 0);

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_FBOs[index]);
    int status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if (status == GL_FRAMEBUFFER_COMPLETE)
    {
        glBindVertexArray(VAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, sourceTex);
        glBindSampler(0, m_sampler);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glBindTexture(GL_TEXTURE_2D,0);
        glUseProgram(0);
    }
    //we have 2048 x 2048 texture filled with single color from captured frame in m_textures[11]

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_FBOs[index]);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, inFBO);
    glReadBuffer(GL_COLOR_ATTACHMENT0);
    glBlitFramebuffer(0,0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_LINEAR);
    --index;
    //rearange channels on input texture form YCbCrA to YYYA if the image is color in YCbCr colorspace
    if(isColor)
    {
        glUseProgram(swizzleProgram);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_FBOs[index]);
        status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if(status == GL_FRAMEBUFFER_COMPLETE)
        {
            glViewport(0,0, m_sizes[index], m_sizes[index]);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, m_textures[index+1]);
            glBindSampler(0, m_sampler);

            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        }
        --index;
    }
    //now m_textures[11] is ready for reduction

    glUseProgram(RedProgram);
    glBindSampler(0, m_sampler);

    for(; index >= 0; --index)
    {
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_FBOs[index]);
        status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

        if(status == GL_FRAMEBUFFER_COMPLETE)
        {
            glViewport(0,0, m_sizes[index], m_sizes[index]);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, m_textures[index+1]);

            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        }
    }
    glBindVertexArray(0);
    glBindFramebuffer(GL_FRAMEBUFFER,0);
}
void Reductor::SwapBuffer() {
    if(m_currentBuffer == m_FBOs[0])
        m_currentBuffer = m_FBOs[1];
    else
        m_currentBuffer = m_FBOs[0];
}
namespace
{

    const char fragmentShaderShiftHueStr[] =
		    "#version 300 es\n"
            "precision mediump float;\n"
            "uniform sampler2D u_Sampler;\n"
            "in vec2 v_TexCoordinates;\n"
            "out vec4 v_FragColor;\n"
            "void main() {\n"
            "   vec4 color = texture(u_Sampler, v_TexCoordinates);\n"
            "   if(color.r > 75.0/360.0 && color.r < 170.0 / 360.0)\n"
            "       color.g -= 0.5;\n"
			"	clamp(color.g, 0.0, 1.0);\n"
            "   v_FragColor = color;\n"
            "   }\n";
    const char fragmentShaderCataractViewStr[] =
            "#version 300 es\n"
		            "precision mediump float;\n"
                    "uniform sampler2D u_Sampler1;\n"
                    "uniform sampler2D u_Sampler2;\n"
                    "in vec2 v_TexCoordinates;\n"
                    "out vec4 v_FragColor;\n"
                    "void main() {\n"
                    "   vec4 view = texture(u_Sampler1, v_TexCoordinates);\n"
                    "   vec4 cataract = texture(u_Sampler2, v_TexCoordinates);\n"
                    "   v_FragColor = vec4 (view.rgb + (cataract.rgb * 1.5), 1.0);\n"
                    "}\n";
    const char fragmentShaderRGB2HSVStr[] =
            "#version 300 es\n"
                "precision mediump float;\n"
                "uniform sampler2D u_Sampler;\n"
                "in vec2 v_TexCoordinates;\n"
                "out vec4 v_FragColor;\n"
                "const vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);\n"
                "void main() {\n"
                    "vec4 c = texture(u_Sampler, v_TexCoordinates);\n"
                    "vec4 p = mix(vec4(c.bg, K.wz), vec4(c.gb, K.xy), step(c.b, c.g));\n"
                    "    vec4 q = mix(vec4(p.xyw, c.r), vec4(c.r, p.yzx), step(p.x, c.r));\n"
                    "    float d = q.x - min(q.w, q.y);\n"
                    "    float e = 1.0e-10;\n"
                    "v_FragColor = vec4(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x, 1.0);\n"
                "}\n";

    const char fragmentShaderHSV2RGBStr[] =
            "#version 300 es\n"
                    "precision mediump float;\n"
                    "uniform sampler2D u_Sampler;\n"
                    "in vec2 v_TexCoordinates;\n"
                    "out vec4 v_FragColor;\n"
                    "const vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);\n"
                    "void main() {\n"
                    "vec4 c = texture(u_Sampler, v_TexCoordinates);\n"
                    "vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);"
                    "v_FragColor = vec4 (c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y), 1.0);"
                    "}\n";

    const char fragmentShaderYCbCr2RGBStr[] = // Y[1.0 - 0.0], Cb/Cr[1.0 - 0.0] RGB [1.0 - 0.0]
            "#version 300 es\n"
                    "precision mediump float;\n"
                    "uniform sampler2D u_Sampler;\n"
                    "const mat3 RGB = mat3(1.00, 0.000, 1.400,\n"
                    "                      1.00,-0.343,-0.711,\n"
                    "                      1.00, 1.765, 0.000);\n"
                    "const vec3 ADJ = vec3(0.00, 0.5, 0.5);\n"
                    "in vec2 v_TexCoordinates;\n"
                    "out vec4 v_FragColor;\n"
                    "void main() {\n"
                    "   v_FragColor = vec4(RGB * (texture(u_Sampler, v_TexCoordinates).rgb - ADJ), 1.0);\n"
                    "}\n";
    const char fragmentShaderRGB2YCbCrStr[] = // Y[1.0 - 0.0], Cb/Cr[1.0 - 0.0] RGB [1.0 - 0.0]
            "#version 300 es\n"
                    "precision mediump float;\n"
                    "uniform sampler2D u_Sampler;\n"
                    "const mat4 YUV = mat4(0.299, 0.587, 0.114, 0.0,\n"
                    "                     -0.169,-0.331, 0.500, 0.0,\n"
                    "                      0.500,-0.419,-0.081, 0.0,\n"
                    "                      0.000, 0.500, 0.500, 1.00);\n"
                    "in vec2 v_TexCoordinates;\n"
                    "out vec4 v_FragColor;\n"
                    "void main() {\n"
                    "   v_FragColor = YUV * texture(u_Sampler, v_TexCoordinates) ;\n"
                    "}\n";

    const char fragmentShaderFillStr[] =
            "#version 300 es\n"
        "precision mediump float;\n"
        "uniform sampler2D u_Sampler;\n"
        "out vec4 o_Max;\n"
        "void main() {\n"
        "   o_Max = texture(u_Sampler, vec2(0.1, 0.1));\n" //luminance on other colorspaces
        "}\n";

    //r channel contains min luminance, r - max luminance
    const char fragmentShaderParallelReductionMinMaxStr[] =
            "#version 300 es\n"
        "precision mediump float;\n"
        "uniform sampler2D u_Sampler;\n"
        "in vec2 v_TexCoordinates;\n"
        "out vec4 v_MinMax;\n"
        "void main() {\n"
        "   vec2 v0 = texture(u_Sampler, v_TexCoordinates).rg;\n"
        "   vec2 v1 = textureOffset (u_Sampler, v_TexCoordinates, ivec2(0, 1)).rg;\n"
        "   vec2 v2 = textureOffset (u_Sampler, v_TexCoordinates, ivec2(1, 0)).rg;\n"
        "   vec2 v3 = textureOffset (u_Sampler, v_TexCoordinates, ivec2(1, 1)).rg;\n"
        "   float o_Min = min(min(v0.r, v1.r), min(v2.r, v3.r));\n"
        "   float o_Max = max(max(v0.g, v1.g), max(v2.g, v3.g));\n"
        "   v_MinMax = vec4(o_Min, o_Max, o_Max, 1.0);\n"
        "}\n";

    const char fragmentShaderParallelReductionWithSwizzleStr[] =
            "#version 300 es\n"
                    "precision mediump float;\n"
                    "uniform sampler2D u_Sampler;\n"
                    "in vec2 v_TexCoordinates;\n"
                    "out vec4 v_MinMax;\n"
                    "void main() {\n"
                    "   vec3 v0 = texture(u_Sampler, v_TexCoordinates).rgb;\n"
                    "   vec3 v1 = textureOffset (u_Sampler, v_TexCoordinates, ivec2(0, 1)).rgb;\n"
                    "   vec3 v2 = textureOffset (u_Sampler, v_TexCoordinates, ivec2(1, 0)).rgb;\n"
                    "   vec3 v3 = textureOffset (u_Sampler, v_TexCoordinates, ivec2(1, 1)).rgb;\n"
                    "   float o_Min = min(min(v0.b, v1.b), min(v2.b, v3.b));\n"
                    "   float o_Max = max(max(v0.b, v1.b), max(v2.b, v3.b));\n"
                    "   v_MinMax = vec4(o_Min, o_Max, o_Max, 1.0);\n"
                    "}\n";

    const char fragmentShaderLinearContrastStretchStr[] =
            "#version 300 es\n"
        "precision mediump float;\n"
        "uniform sampler2D u_Sampler1;\n"   //min in r channel, max in g channel - luminance value
        "uniform sampler2D u_Sampler2;\n"   //input image
        "in vec2 v_TexCoordinates;\n"
        "out vec4 o_FragColor;\n"
        "void main() {\n"
        "   vec2 MinMax = texture(u_Sampler1, vec2(1.0, 1.0)).rg;\n"
        "   float texel = texture(u_Sampler2, v_TexCoordinates).r;\n"
        "   float color = (texel - MinMax.r) * ((1.0 - 0.00) / (MinMax.g - MinMax.r)) + 0.00;\n"
        "   o_FragColor = vec4(color, color, color, 1.0);\n"
        "}\n";

    const char fragmentShaderLinearColorContrastStretchStr[] =
            "#version 300 es\n"
                    "precision mediump float;\n"
                    "uniform sampler2D u_Sampler1;\n"   //min in r channel, max in g channel - luminance value
                    "uniform sampler2D u_Sampler2;\n"   //input image
                    "uniform float u_Min;\n"
                    "uniform float u_Max;\n"
                    "const float f_MAX = 0.75;\n"        //maximum luminance value that current colorspace supports
                    "const float f_MIN = 0.15;\n"        //minimum luminance value that current colorspace supports
                    "in vec2 v_TexCoordinates;\n"
                    "out vec4 o_FragColor;\n"
                    "void main() {\n"
                    "   vec2 MinMax = texture(u_Sampler1, vec2(1.0, 1.0)).rg;\n"
                    "   vec4 texel = texture(u_Sampler2, v_TexCoordinates);\n"
                    "   float color = (texel.b - MinMax.r) * ((u_Max - u_Min) / (MinMax.g - MinMax.r)) + u_Min;\n" //lineary stretch luminance which on HSV image is stored in b
                    "   o_FragColor = vec4(texel.r, texel.g, color, 1.0);\n"
                    "}\n";

    const char vertexShaderBaseStr[] =
        "#version 300 es\n"
        "layout(location = 0) in vec4 a_Position;\n"
        "layout(location = 1) in vec2 a_TexCoordinates;\n"
        "out vec2 v_TexCoordinates;\n"
        "void main() {\n"
        "    gl_Position = a_Position;\n"
        "    v_TexCoordinates = a_TexCoordinates;\n"
        "}\n";

    const char fragmentShaderGrayScaleStr[] =
        "#version 300 es\n"
        "precision mediump float;\n"
        "uniform sampler2D u_Sampler;\n"
        "const vec4 cv_GRAYSCALE = vec4(0.2126, 0.7152, 0.0722, 0.0);\n"
        "in vec2 v_TexCoordinates;\n"
        "layout(location = 0) out vec4 o_FragColor;\n"
        "void main() {\n"
        "    float average = dot(cv_GRAYSCALE, texture(u_Sampler, v_TexCoordinates));\n"
        "    o_FragColor = vec4(average, average, average, 1.0);\n"
        "}\n";

    const char fragmentShaderProtanopiaStr[] =
            "#version 300 es\n"
            "precision mediump float;\n"
            "uniform sampler2D u_Sampler;\n"
            "in vec2 v_TexCoordinates;\n"
            "layout(location = 0) out vec4 o_FragColor;\n"
            "void main() {\n"
            "    o_FragColor = texture(u_Sampler, v_TexCoordinates);\n"
            "    vec3 colorRGB = o_FragColor.rgb;\n"
            "    colorRGB *= mat3(0.567, 0.433, 0.0  ,  0.558, 0.442, 0.0  ,  0.0  , 0.242, 0.758);\n"
            "    o_FragColor = vec4( colorRGB.x , colorRGB.y, colorRGB.z, 1.0 );\n"
            "}\n";

    //Applies single pass gaussian blur with kernel 3x3. Needs to be applied twice - vertically and horizontally
    const char fragmentShaderGaussianBlurStr[] =
        "#version 300 es\n"
        "precision mediump float;\n"
        "uniform sampler2D u_Sampler;\n"
        "in vec2 v_TexCoordinates;\n"
        "layout(location = 0) out vec4 o_FragColor;\n"
        "uniform float u_Resolution;\n"
        "uniform float u_Radius;\n"
        "uniform vec2 u_Direction;\n"
        "void main() {\n"
        "    vec4 sum = vec4(0.0);\n"
        "    float blur = u_Radius/u_Resolution;\n"
        "    sum += texture(u_Sampler, vec2(v_TexCoordinates.x - 1.0 * blur * u_Direction.x, v_TexCoordinates.y - 1.0 * blur * u_Direction.y)) * 0.27901;\n"
        "    sum += texture(u_Sampler, vec2(v_TexCoordinates.x, v_TexCoordinates.y)) * 0.44198;\n"
        "    sum += texture(u_Sampler, vec2(v_TexCoordinates.x + 1.0 * blur * u_Direction.x, v_TexCoordinates.y + 1.0 * blur * u_Direction.y)) * 0.27901;\n"
        "    o_FragColor = vec4(sum.rgb, 1.0f);\n"
        "}\n";

    //Applies sobel edge detection on a image. Ideally it should sample from blurred, grayscale image to avoid noise amplification.
    //Shader then mixes original image with image filtered by sobel in proportion specified by "amount" uniform
    static const char fragmentShaderSobelStr[] =
        "#version 300 es\n"
        "precision mediump float;\n"
        "uniform sampler2D u_Sampler1;\n"  //original image
        "uniform sampler2D u_Sampler2;\n"  //blurred, grayscale image
        "uniform float u_Amount;\n"
        "in vec2 v_TexCoordinates;\n"
        "layout(location = 0) out vec4 o_FragColor;\n"
        "void main()\n"
        "{\n"
        "   vec2 texel = vec2(textureSize(u_Sampler1, 0));"
        "   float x = 1.0 / texel.x;\n"
        "   float y = 1.0 / texel.y;\n"
        "   vec4 horizEdge = vec4( 0.0 );\n"
        "   horizEdge -= texture( u_Sampler2, vec2( v_TexCoordinates.x - x, v_TexCoordinates.y - y ) ) * 1.0;\n"
        "   horizEdge -= texture( u_Sampler2, vec2( v_TexCoordinates.x - x, v_TexCoordinates.y     ) ) * 2.0;\n"
        "   horizEdge -= texture( u_Sampler2, vec2( v_TexCoordinates.x - x, v_TexCoordinates.y + y ) ) * 1.0;\n"
        "   horizEdge += texture( u_Sampler2, vec2( v_TexCoordinates.x + x, v_TexCoordinates.y - y ) ) * 1.0;\n"
        "   horizEdge += texture( u_Sampler2, vec2( v_TexCoordinates.x + x, v_TexCoordinates.y     ) ) * 2.0;\n"
        "   horizEdge += texture( u_Sampler2, vec2( v_TexCoordinates.x + x, v_TexCoordinates.y + y ) ) * 1.0;\n"
        "   vec4 vertEdge = vec4( 0.0 );\n"
        "   vertEdge -= texture( u_Sampler2, vec2( v_TexCoordinates.x - x, v_TexCoordinates.y - y ) ) * 1.0;\n"
        "   vertEdge -= texture( u_Sampler2, vec2( v_TexCoordinates.x    , v_TexCoordinates.y - y ) ) * 2.0;\n"
        "   vertEdge -= texture( u_Sampler2, vec2( v_TexCoordinates.x + x, v_TexCoordinates.y - y ) ) * 1.0;\n"
        "   vertEdge += texture( u_Sampler2, vec2( v_TexCoordinates.x - x, v_TexCoordinates.y + y ) ) * 1.0;\n"
        "   vertEdge += texture( u_Sampler2, vec2( v_TexCoordinates.x    , v_TexCoordinates.y + y ) ) * 2.0;\n"
        "   vertEdge += texture( u_Sampler2, vec2( v_TexCoordinates.x + x, v_TexCoordinates.y + y ) ) * 1.0;\n"
        "   vec3 edge = sqrt((horizEdge.rgb * horizEdge.rgb) + (vertEdge.rgb * vertEdge.rgb));\n"
        //mix original image with sobel operator results
        "   vec4 orig = texture( u_Sampler1, v_TexCoordinates);\n"
        "   o_FragColor = (1.0 - u_Amount) * orig + (u_Amount) * (vec4( edge, 1.0 )) ;\n"
        "}\n";

    //Takes to textures: 1. original image from camera and 2. blurred image.
    //Then it produces the mask and applies mask on original image producing sharp image with proportion specified by uniform "amount".
    static const char fragmentShaderUnsharpMaskStr[] =
        "#version 300 es\n"
        "precision mediump float;\n"
        "uniform sampler2D u_Sampler1;\n" //original image
        "uniform sampler2D u_Sampler2;\n" //blurred image
        "uniform float u_Amount;\n"
        "in vec2 v_TexCoordinates;\n"
        "layout(location = 0) out vec4 o_FragColor;\n"
        "void main()\n"
        "{\n"
        "   vec4 sharp = texture(u_Sampler1, v_TexCoordinates);\n"
        "   vec4 blurred = texture(u_Sampler2, v_TexCoordinates);\n" //compute mask
        "   o_FragColor = sharp + ((sharp - blurred) * u_Amount);\n"
        "}\n";

    //accepts grayscale image but it can work on color too.
    static const char fragmentShaderBinaryThresholdStr[] =
        "#version 300 es\n"
        "precision mediump float;\n"
        "uniform sampler2D u_Sampler;\n"
        "in vec2 v_TexCoordinates;\n"
        "out vec4 o_FragColor;\n"
        "uniform float u_Threshold;\n"
        "uniform float u_Invert;\n"
        "void main()\n"
        "{\n"
        "   vec4 pixel = texture(u_Sampler, v_TexCoordinates);\n"
        "   float thresh = abs( (u_Invert * 1.0) - u_Threshold );\n"
        "   if(pixel.g >= thresh)\n"
        "   { o_FragColor = vec4(1.0); }\n"
        "   else\n"
        "   { o_FragColor = vec4(0.0); }\n"
        "}\n";

    //should be used on a grayscale image with kernel 3x3
    static const char fragmentShaderAdaptiveThresholdStr[] =
        "#version 300 es\n"
        "precision mediump float;\n"
        "uniform sampler2D u_Sampler;\n"
        "in vec2 v_TexCoordinates;\n"
        "out vec4 o_FragColor;\n"
        "uniform float u_C;\n"
        "uniform float u_Invert;\n" //determines if we should invert thresholding
        "uniform float u_Offset;\n" //this will be computed in Apply function (kerneSize / 2) and sent as uniform
        "void main()\n"
        "{\n"
        "   vec2 texel = vec2(textureSize(u_Sampler, 0));" //obtain texture resolution
        "   float stepx = 1.0 / texel.x;\n"
        "   float stepy = 1.0 / texel.y;\n"
        "   float thresh;\n"
        "   vec4 mean = vec4(0.0);\n"
        "   for(float i = -u_Offset; i <= u_Offset; ++i) {\n"
        "       for(float j = -u_Offset; j <= u_Offset; ++j) {\n"
        "           mean += texture( u_Sampler, vec2( v_TexCoordinates.x + stepx * i, v_TexCoordinates.y + stepy *j ) );\n"
        "       }\n"
        "   }\n"
        "   thresh = abs( (u_Invert * 1.0) - ((mean.g / (pow ( (u_Offset * 2.0 + 1.0) , 2.0)) )  - u_C));\n" //mean / kernelSize^2 (pow ( (u_Offset * 2 + 1) , 2))
        "   vec4 centralPixel = texture(u_Sampler, v_TexCoordinates);\n"
        "   if(centralPixel.g >= thresh)\n"
        "       {o_FragColor = vec4(1.0);}\n"
        "   else\n"
        "   {o_FragColor = vec4(0.0);}\n"
        "}\n";

}

extern "C" {

void drawShiftHue(GLuint inTextureId, int texFboIndex)
{
	glUseProgram(programShiftHue);
	glViewport(0, 0, frameBufferWidth, frameBufferHeight);

	GLuint samplerParam = glGetUniformLocation(programShiftHue, "u_Sampler");
	glUniform1i(samplerParam, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffers[texFboIndex]);

	int status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (status == GL_FRAMEBUFFER_COMPLETE)
	{
		glBindVertexArray(VAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, inTextureId);
		glBindSampler(0, samplers[0]);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
}
void drawProtanopia(GLuint inTextureId, int texFboIndex)
{
    glUseProgram(programProtanopia);
    glViewport(0, 0, frameBufferWidth, frameBufferHeight);

    GLuint samplerParam = glGetUniformLocation(programProtanopia, "u_Sampler");
    glUniform1i(samplerParam, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffers[texFboIndex]);

    int status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if (status == GL_FRAMEBUFFER_COMPLETE)
    {
        glBindVertexArray(VAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, inTextureId);
        glBindSampler(0, samplers[0]);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
}

void drawGrayScale(GLuint inTextureId, int texFboIndex)
{
    glUseProgram(programGrayScale);
    glViewport(0, 0, frameBufferWidth, frameBufferHeight);

    GLuint samplerParam = glGetUniformLocation(programGrayScale, "u_Sampler");
    glUniform1i(samplerParam, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffers[texFboIndex]);

    int status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if (status == GL_FRAMEBUFFER_COMPLETE)
    {
        glBindVertexArray(VAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, inTextureId);
        glBindSampler(0, samplers[0]);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
}

void drawGaussianBlur(float radius, int count, GLuint inTextureId,
                      int texFboIndex1,
                      int texFboIndex2)
{
    bool firstTime = true;
    glUseProgram(programGaussianBlur);
    glViewport(0, 0, frameBufferWidth, frameBufferHeight);

    GLuint samplerParam = glGetUniformLocation(programGaussianBlur, "u_Sampler");
    GLuint resolutionParam = (GLuint) glGetUniformLocation(programGaussianBlur, "u_Resolution");
    GLuint radiusParam = (GLuint) glGetUniformLocation(programGaussianBlur, "u_Radius");
    GLuint directionParam = (GLuint) glGetUniformLocation(programGaussianBlur, "u_Direction");

    glUniform1i(samplerParam, 0);

    for (int i = 0; i < count; ++i)
    {
        float horizontal = 1.0f;
        float vertical = 0.0f;

        glBindFramebuffer(GL_FRAMEBUFFER, frameBuffers[texFboIndex1]);

        glActiveTexture(GL_TEXTURE0);
        if (firstTime)
        {
            glBindTexture(GL_TEXTURE_2D, inTextureId);
            firstTime = false;
        }
        else
        {
            glBindTexture(GL_TEXTURE_2D, textures[texFboIndex2]);
        }
        glBindSampler(0, samplers[0]);

        glUniform1f(resolutionParam,  (float) frameBufferHeight);
        glUniform1f(radiusParam, radius);
        glUniform2f(directionParam, horizontal, vertical);

        int status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

        if (status == GL_FRAMEBUFFER_COMPLETE)
        {
            glBindVertexArray(VAO);

            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

            glBindVertexArray(0);
        }

        horizontal = 0.0f;
        vertical = 1.0f;

        glBindFramebuffer(GL_FRAMEBUFFER, frameBuffers[texFboIndex2]);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textures[texFboIndex1]);
        glBindSampler(0, samplers[0]);

        glUniform1f(resolutionParam,  (float) frameBufferWidth);
        glUniform1f(radiusParam, radius);
        glUniform2f(directionParam, horizontal, vertical);

        status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

        if (status == GL_FRAMEBUFFER_COMPLETE)
        {
            glBindVertexArray(VAO);

            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

            glBindVertexArray(0);
        }
    }
}

void drawSobel(float amount, GLuint inTextureId, int texFboIndex1, int texFboIndex2)
{
    glUseProgram(programSobel);
    glViewport(0, 0, frameBufferWidth, frameBufferHeight);

    GLuint sampler1Param = glGetUniformLocation(programSobel, "u_Sampler1");
    GLuint sampler2Param = glGetUniformLocation(programSobel, "u_Sampler2");
    GLuint amountParam = glGetUniformLocation(programSobel, "u_Amount");

    glUniform1f(amountParam, amount);
    glUniform1i(sampler1Param, 0);
    glUniform1i(sampler2Param, 1);

    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffers[texFboIndex2]);

    int status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if (status == GL_FRAMEBUFFER_COMPLETE)
    {
        glBindVertexArray(VAO);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, inTextureId);
        glBindSampler(0, samplers[0]);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, textures[texFboIndex1]);
        glBindSampler(1, samplers[0]);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);
    }
}

void drawUnsharpMask(float amount, GLuint inTextureId, int texFboIndex1, int texFboIndex2)
{
    glUseProgram(programUnsharpMask);
    glViewport(0, 0, frameBufferWidth, frameBufferHeight);

    GLuint sampler1Param = glGetUniformLocation(programUnsharpMask, "u_Sampler1");
    GLuint sampler2Param = glGetUniformLocation(programUnsharpMask, "u_Sampler2");
    GLuint amountParam = glGetUniformLocation(programUnsharpMask, "u_Amount");

    glUniform1f(amountParam, amount);
    glUniform1i(sampler1Param, 0);
    glUniform1i(sampler2Param, 1);

    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffers[texFboIndex2]);

    int status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if (status == GL_FRAMEBUFFER_COMPLETE)
    {
        glBindVertexArray(VAO);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, inTextureId);
        glBindSampler(0, samplers[0]);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, textures[texFboIndex1]);
        glBindSampler(1, samplers[0]);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);
    }
}

void drawCataractView(GLuint inTextureId, GLuint inCataractTexId, int texFboIndex)
{
    glUseProgram(programCataractView);
    glViewport(0, 0, frameBufferWidth, frameBufferHeight);

    GLuint sampler1Param = glGetUniformLocation(programCataractView, "u_Sampler1");
    GLuint sampler2Param = glGetUniformLocation(programCataractView, "u_Sampler2");

    glUniform1i(sampler1Param, 0);
    glUniform1i(sampler2Param, 1);

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, frameBuffers[texFboIndex]);

    int status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if (status == GL_FRAMEBUFFER_COMPLETE)
    {
        glBindVertexArray(VAO);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, inTextureId);
        glBindSampler(0, samplers[0]);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, inCataractTexId);
        glBindSampler(1, samplers[0]);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);
    }
}

void drawBinaryThreshold(float threshold, float type, GLuint inTextureId, int texFboIndex)
{
    glUseProgram(programBinaryThreshold);
    glViewport(0, 0, frameBufferWidth, frameBufferHeight);

    GLuint samplerParam = glGetUniformLocation(programBinaryThreshold, "u_Sampler");
    GLuint tresholdParam = glGetUniformLocation(programBinaryThreshold, "u_Threshold");
    GLuint invertParam = glGetUniformLocation(programBinaryThreshold, "u_Invert");

    glUniform1i(samplerParam, 0);
    glUniform1f(tresholdParam, threshold);
    glUniform1f(invertParam, type);

    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffers[texFboIndex]);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT );

    int status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if (status == GL_FRAMEBUFFER_COMPLETE)
    {
        glBindVertexArray(VAO);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, inTextureId);
        glBindSampler(0, samplers[0]);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glBindVertexArray(0);
    }
}

void drawAdaptiveThreshold(int kernelSize, float C, float type, GLuint inTextureId, int texFboIndex)
{
    glUseProgram(programAdaptiveThreshold);
    glViewport(0, 0, frameBufferWidth, frameBufferHeight);

    GLuint samplerParam = glGetUniformLocation(programAdaptiveThreshold, "u_Sampler");
    GLuint offsetParam = glGetUniformLocation(programAdaptiveThreshold, "u_Offset");
    GLuint cParam = glGetUniformLocation(programAdaptiveThreshold, "u_C");
    GLuint invertParam = glGetUniformLocation(programAdaptiveThreshold, "u_Invert");

    glUniform1i(samplerParam, 0);
    glUniform1f(offsetParam, float (kernelSize / 2));
    glUniform1f(cParam, C);
    glUniform1f(invertParam, type);

    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffers[texFboIndex]);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT );

    int status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if (status == GL_FRAMEBUFFER_COMPLETE)
    {
        glBindVertexArray(VAO);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, inTextureId);
        glBindSampler(0, samplers[0]);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);
    }
}

void drawContrastStretch(GLuint inTextureId, GLuint MinaMaxTexId, int texFboIndex)
{
    glUseProgram(programContrastStretch);
    glViewport(0, 0, frameBufferWidth, frameBufferHeight);

    GLuint sampler1Param = glGetUniformLocation(programContrastStretch, "u_Sampler1");
    GLuint sampler2Param = glGetUniformLocation(programContrastStretch, "u_Sampler2");

    glUniform1i(sampler1Param, 0);
    glUniform1i(sampler2Param, 1);

    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffers[texFboIndex]);

    int status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if (status == GL_FRAMEBUFFER_COMPLETE)
    {
        glBindVertexArray(VAO);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, MinaMaxTexId);
        glBindSampler(0, samplers[0]);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, inTextureId);
        glBindSampler(1, samplers[0]);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);
    }
}

//destMin - minimum value to which luminance will be stretched/shrunk, destMax - maximum
void drawColorContrastStretch(GLuint inTextureId, GLuint MinaMaxTexId, int texFboIndex, float destMin = 0.0f,float destMax = 1.0)
{
    glUseProgram(programColorContrastStretch);
    glViewport(0, 0, frameBufferWidth, frameBufferHeight);

    GLuint sampler1Param = glGetUniformLocation(programColorContrastStretch, "u_Sampler1");
    GLuint sampler2Param = glGetUniformLocation(programColorContrastStretch, "u_Sampler2");
    GLuint minParam = glGetUniformLocation(programColorContrastStretch, "u_Min");
    GLuint maxParam = glGetUniformLocation(programColorContrastStretch, "u_Max");

    glUniform1i(sampler1Param, 0);
    glUniform1i(sampler2Param, 1);
    glUniform1f(minParam, destMin);
    glUniform1f(maxParam, destMax);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffers[texFboIndex]);

    int status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if (status == GL_FRAMEBUFFER_COMPLETE)
    {
        glBindVertexArray(VAO);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, MinaMaxTexId);
        glBindSampler(0, samplers[0]);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, inTextureId);
        glBindSampler(1, samplers[0]);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);
    }
}

void drawRGB2YCbCr(GLuint inTextureId, int texFboIndex)
{
    glUseProgram(programRGB2YCbCr);
    glViewport(0, 0, frameBufferWidth, frameBufferHeight);

    GLuint samplerParam = glGetUniformLocation(programRGB2YCbCr, "u_Sampler");
    glUniform1i(samplerParam, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffers[texFboIndex]);

    int status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if (status == GL_FRAMEBUFFER_COMPLETE)
    {
        glBindVertexArray(VAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, inTextureId);
        glBindSampler(0, samplers[0]);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
}

void drawYCbCr2RGB(GLuint inTextureId, int texFboIndex)
{
    glUseProgram(programYCbCr2RGB);
    glViewport(0, 0, frameBufferWidth, frameBufferHeight);

    GLuint samplerParam = glGetUniformLocation(programYCbCr2RGB, "u_Sampler");
    glUniform1i(samplerParam, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffers[texFboIndex]);

    int status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if (status == GL_FRAMEBUFFER_COMPLETE)
    {
        glBindVertexArray(VAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, inTextureId);
        glBindSampler(0, samplers[0]);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
}

void drawRGB2HSV(GLuint inTextureId, int texFboIndex)
{
    glUseProgram(programRGB2HSV);
    glViewport(0, 0, frameBufferWidth, frameBufferHeight);

    GLuint samplerParam = glGetUniformLocation(programRGB2HSV, "u_Sampler");
    glUniform1i(samplerParam, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffers[texFboIndex]);

    int status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if (status == GL_FRAMEBUFFER_COMPLETE)
    {
        glBindVertexArray(VAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, inTextureId);
        glBindSampler(0, samplers[0]);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
}

void drawHSV2RGB(GLuint inTextureId, int texFboIndex)
{
    glUseProgram(programHSV2RGB);
    glViewport(0, 0, frameBufferWidth, frameBufferHeight);

    GLuint samplerParam = glGetUniformLocation(programHSV2RGB, "u_Sampler");
    glUniform1i(samplerParam, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffers[texFboIndex]);

    int status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if (status == GL_FRAMEBUFFER_COMPLETE)
    {
        glBindVertexArray(VAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, inTextureId);
        glBindSampler(0, samplers[0]);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
}
void
Java_com_samsung_mobile_daydreamtemplate_MainActivity_initResources(
        JNIEnv *env,
        jobject,
        jint width,
        jint height)
{
    //VAO
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &vertPosBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertPosBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(QUAD_COORDINATES), QUAD_COORDINATES, GL_STATIC_DRAW);
    glVertexAttribPointer(VERTEX_POSITION_INDEX, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),(GLvoid*) 0);
    glEnableVertexAttribArray(VERTEX_POSITION_INDEX);

    glGenBuffers(1, &texCoordBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, texCoordBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(QUAD_TEX_COORDINATES), QUAD_TEX_COORDINATES, GL_STATIC_DRAW);
    glVertexAttribPointer(TEXTURE_COORD_INDEX, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (GLvoid*) 0);
    glEnableVertexAttribArray(TEXTURE_COORD_INDEX);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(QUAD_VERT_INDICES), QUAD_VERT_INDICES, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    glBindVertexArray(0);
    reductor.Create();

    frameBufferWidth = (GLuint) width;
    frameBufferHeight = (GLuint) height;

    glGenTextures(TEX_FBO_COUNT, textures);
    glGenFramebuffers(TEX_FBO_COUNT, frameBuffers);
    glGenSamplers(SAMPLERS_COUNT, samplers);

    for (int i = 0; i < TEX_FBO_COUNT; ++i)
    {
        glBindTexture(GL_TEXTURE_2D, textures[i]);
        glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB8, frameBufferWidth, frameBufferHeight);

        glBindFramebuffer(GL_FRAMEBUFFER, frameBuffers[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textures[i], 0);
    }

    glSamplerParameteri(samplers[0], GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glSamplerParameteri(samplers[0], GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glSamplerParameteri(samplers[0], GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glSamplerParameteri(samplers[0], GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    GLuint vertexShaderBase = loadShader(GL_VERTEX_SHADER, vertexShaderBaseStr);
    GLuint fragmentShaderGrayScale = loadShader(GL_FRAGMENT_SHADER, fragmentShaderGrayScaleStr);
    GLuint fragmentShaderProtanopia = loadShader(GL_FRAGMENT_SHADER, fragmentShaderProtanopiaStr);
    GLuint fragmentShaderGaussianBlur = loadShader(GL_FRAGMENT_SHADER, fragmentShaderGaussianBlurStr);
    GLuint fragmentShaderSobel = loadShader(GL_FRAGMENT_SHADER, fragmentShaderSobelStr);
    GLuint fragmentShaderUnsharpMask = loadShader(GL_FRAGMENT_SHADER, fragmentShaderUnsharpMaskStr);
    GLuint fragmentShaderBinaryThreshold = loadShader(GL_FRAGMENT_SHADER, fragmentShaderBinaryThresholdStr);
    GLuint fragmentShaderAdaptiveThreshold = loadShader(GL_FRAGMENT_SHADER, fragmentShaderAdaptiveThresholdStr);
    GLuint fragmentShaderParallelReductionMinMax = loadShader(GL_FRAGMENT_SHADER, fragmentShaderParallelReductionMinMaxStr);
    GLuint fragmentShaderReduceWithSwizzle = loadShader(GL_FRAGMENT_SHADER, fragmentShaderParallelReductionWithSwizzleStr);
    GLuint fragmentShaderFill = loadShader(GL_FRAGMENT_SHADER, fragmentShaderFillStr);
    GLuint fragmentShaderLinearContrastStretch = loadShader(GL_FRAGMENT_SHADER, fragmentShaderLinearContrastStretchStr);
    GLuint fragmentShaderColorContrastStretch = loadShader(GL_FRAGMENT_SHADER, fragmentShaderLinearColorContrastStretchStr);
    GLuint fragmentShaderRGB2YCbCr = loadShader(GL_FRAGMENT_SHADER, fragmentShaderRGB2YCbCrStr);
    GLuint fragmentShaderYCbCr2RGB = loadShader(GL_FRAGMENT_SHADER, fragmentShaderYCbCr2RGBStr);
    GLuint fragmentShaderRGB2HSV = loadShader(GL_FRAGMENT_SHADER, fragmentShaderRGB2HSVStr);
    GLuint fragmentShaderHSV2RGB = loadShader(GL_FRAGMENT_SHADER, fragmentShaderHSV2RGBStr);
    GLuint fragmentShaderCataractView = loadShader(GL_FRAGMENT_SHADER, fragmentShaderCataractViewStr);
	GLuint fragmentShaderShiftHue = loadShader(GL_FRAGMENT_SHADER, fragmentShaderShiftHueStr);

    programGrayScale = glCreateProgram();
    glAttachShader(programGrayScale, vertexShaderBase);
    glAttachShader(programGrayScale, fragmentShaderGrayScale);
    glLinkProgram(programGrayScale);
    glDeleteShader(fragmentShaderGrayScale);

    programProtanopia = glCreateProgram();
    glAttachShader(programProtanopia, vertexShaderBase);
    glAttachShader(programProtanopia, fragmentShaderProtanopia);
    glLinkProgram(programProtanopia);

    programGaussianBlur = glCreateProgram();
    glAttachShader(programGaussianBlur, vertexShaderBase);
    glAttachShader(programGaussianBlur, fragmentShaderGaussianBlur);
    glLinkProgram(programGaussianBlur);
    glDeleteShader(fragmentShaderGaussianBlur);

    programSobel = glCreateProgram();
    glAttachShader(programSobel, vertexShaderBase);
    glAttachShader(programSobel, fragmentShaderSobel);
    glLinkProgram(programSobel);
    glDeleteShader(fragmentShaderSobel);

    programUnsharpMask = glCreateProgram();
    glAttachShader(programUnsharpMask, vertexShaderBase);
    glAttachShader(programUnsharpMask, fragmentShaderUnsharpMask);
    glLinkProgram(programUnsharpMask);
    glDeleteShader(fragmentShaderUnsharpMask);

    programBinaryThreshold = glCreateProgram();
    glAttachShader(programBinaryThreshold, vertexShaderBase);
    glAttachShader(programBinaryThreshold, fragmentShaderBinaryThreshold);
    glLinkProgram(programBinaryThreshold);
    glDeleteShader(fragmentShaderBinaryThreshold);

    programAdaptiveThreshold = glCreateProgram();
    glAttachShader(programAdaptiveThreshold, vertexShaderBase);
    glAttachShader(programAdaptiveThreshold, fragmentShaderAdaptiveThreshold);
    glLinkProgram(programAdaptiveThreshold);
    glDeleteShader(fragmentShaderAdaptiveThreshold);

    programParallelReductionMinMax = glCreateProgram();
    glAttachShader(programParallelReductionMinMax, vertexShaderBase);
    glAttachShader(programParallelReductionMinMax, fragmentShaderParallelReductionMinMax);
    glLinkProgram(programParallelReductionMinMax);
    glDeleteShader(fragmentShaderParallelReductionMinMax);

    programFillTexture = glCreateProgram();
    glAttachShader(programFillTexture, vertexShaderBase);
    glAttachShader(programFillTexture, fragmentShaderFill);
    glLinkProgram(programFillTexture);
    glDeleteShader(fragmentShaderFill);

    programContrastStretch = glCreateProgram();
    glAttachShader(programContrastStretch, vertexShaderBase);
    glAttachShader(programContrastStretch, fragmentShaderLinearContrastStretch);
    glLinkProgram(programContrastStretch);
    glDeleteShader(fragmentShaderLinearContrastStretch);

    programRGB2YCbCr = glCreateProgram();
    glAttachShader(programRGB2YCbCr, vertexShaderBase);
    glAttachShader(programRGB2YCbCr, fragmentShaderRGB2YCbCr);
    glLinkProgram(programRGB2YCbCr);
    glDeleteShader(fragmentShaderRGB2YCbCr);

    programYCbCr2RGB = glCreateProgram();
    glAttachShader(programYCbCr2RGB, vertexShaderBase);
    glAttachShader(programYCbCr2RGB, fragmentShaderYCbCr2RGB);
    glLinkProgram(programYCbCr2RGB);
    glDeleteShader(fragmentShaderYCbCr2RGB);

    programReduceWithSwizzle = glCreateProgram();
    glAttachShader(programReduceWithSwizzle, vertexShaderBase);
    glAttachShader(programReduceWithSwizzle, fragmentShaderReduceWithSwizzle);
    glLinkProgram(programReduceWithSwizzle);
    glDeleteShader(fragmentShaderReduceWithSwizzle);

    programColorContrastStretch = glCreateProgram();
    glAttachShader(programColorContrastStretch, vertexShaderBase);
    glAttachShader(programColorContrastStretch, fragmentShaderColorContrastStretch);
    glLinkProgram(programColorContrastStretch);
    glDeleteShader(fragmentShaderColorContrastStretch);

    programRGB2HSV = glCreateProgram();
    glAttachShader(programRGB2HSV, vertexShaderBase);
    glAttachShader(programRGB2HSV, fragmentShaderRGB2HSV);
    glLinkProgram(programRGB2HSV);
    glDeleteShader(fragmentShaderRGB2HSV);

    programHSV2RGB = glCreateProgram();
    glAttachShader(programHSV2RGB, vertexShaderBase);
    glAttachShader(programHSV2RGB, fragmentShaderHSV2RGB);
    glLinkProgram(programHSV2RGB);
    glDeleteShader(fragmentShaderHSV2RGB);

    programCataractView = glCreateProgram();
    glAttachShader(programCataractView, vertexShaderBase);
    glAttachShader(programCataractView, fragmentShaderCataractView);
    glLinkProgram(programCataractView);
    glDeleteShader(fragmentShaderCataractView);

	programShiftHue = glCreateProgram();
	glAttachShader(programShiftHue, vertexShaderBase);
	glAttachShader(programShiftHue, fragmentShaderShiftHue);
	glLinkProgram(programShiftHue);
	glDeleteShader(fragmentShaderShiftHue);

    glDeleteShader(vertexShaderBase);
}


// 0 - original image
// 1 - cataract vision simulation
// 2 - sobel edge detection + cataract vision simulation
// 3 - unsharp mask + cataract vision simulation
// 4 - unsharp mask + contrast stretch + cataract vision simulation
// 5 - color blindness
// 6 - sobel edge detection
// 7 - unsharp mask

jint Java_com_samsung_mobile_daydreamtemplate_MainActivity_applyFilters(JNIEnv *env,
                                                                        jobject,
                                                                        jint inTextureId,
                                                                        jint inCataractTexId,
                                                                        jint filter)
{
    switch (filter)
    {
        case 0:
            return  inTextureId;
        case 1:
            //simulation of cataract vision
            drawGaussianBlur(1.0, 3,  (GLuint) inTextureId, 0, 1);
            drawRGB2HSV(textures[1], 2);
            reductor.Reduce(VAO, frameBuffers[2], textures[2], frameBufferWidth, frameBufferHeight, programFillTexture, programParallelReductionMinMax, programReduceWithSwizzle, true);
            drawColorContrastStretch(textures[2], reductor.m_textures[0], 1, 0.15, 0.85);
            drawHSV2RGB(textures[1], 2);
            drawCataractView(textures[2], inCataractTexId, 0);
            return textures[0];
        case 2:
            //sobel + cataract view simulation
            drawGrayScale((GLuint) inTextureId, 0);
            drawGaussianBlur(1.0f, 3, textures[0], 1, 2);
            drawSobel(0.5f, (GLuint) inTextureId, 2, 1);
            drawGaussianBlur(1.0, 3,  textures[1], 0, 2);
            drawRGB2HSV(textures[2], 1);
            reductor.Reduce(VAO, frameBuffers[1], textures[1], frameBufferWidth, frameBufferHeight, programFillTexture, programParallelReductionMinMax, programReduceWithSwizzle, true);
            drawColorContrastStretch(textures[1], reductor.m_textures[0], 0, 0.15, 0.85); //here we can change contrast spectrum
            drawHSV2RGB(textures[0], 1);
            drawCataractView(textures[1], inCataractTexId, 0);
            return textures[0];
        case 3:
            //unsharp mask + cataract view
            drawGaussianBlur(1.0f, 3, (GLuint) inTextureId, 0, 1);
            drawUnsharpMask(1.0f, (GLuint) inTextureId, 1, 2);

            drawGaussianBlur(1.0, 3,  textures[2], 0, 1);
            drawRGB2HSV(textures[1], 0);
            reductor.Reduce(VAO, frameBuffers[0], textures[0], frameBufferWidth, frameBufferHeight, programFillTexture, programParallelReductionMinMax, programReduceWithSwizzle, true);
            drawColorContrastStretch(textures[0], reductor.m_textures[0], 1, 0.15, 0.85);
            drawHSV2RGB(textures[1], 0);
            drawCataractView(textures[0], inCataractTexId, 1);
            return textures[1];
        case 4:
            //unsharp mask + contrast stretch + cataract view
            drawGaussianBlur(1.0f, 3, (GLuint) inTextureId, 0, 1);
            drawUnsharpMask(1.0f, (GLuint) inTextureId, 1, 2);
            drawRGB2HSV(textures[2], 0);
            reductor.Reduce(VAO, frameBuffers[0], textures[0], frameBufferWidth, frameBufferHeight, programFillTexture, programParallelReductionMinMax, programReduceWithSwizzle, true);
            drawColorContrastStretch(textures[0], reductor.m_textures[0], 1, 0.00, 1.00);

            reductor.Reduce(VAO, frameBuffers[1], textures[1], frameBufferWidth, frameBufferHeight, programFillTexture, programParallelReductionMinMax, programReduceWithSwizzle, true);
            drawColorContrastStretch(textures[1], reductor.m_textures[0], 0, 0.15, 0.85);
            drawHSV2RGB(textures[0], 1);
            drawGaussianBlur(1.0, 3,  textures[1], 0, 2);
            drawCataractView(textures[2], inCataractTexId, 0);
            return textures[0];
        case 5:
            drawProtanopia((GLuint) inTextureId, 0);
            return textures[0];
        case 6:
            drawRGB2HSV(inTextureId, 0);
            reductor.Reduce(VAO, frameBuffers[0], textures[0], frameBufferWidth, frameBufferHeight, programFillTexture, programParallelReductionMinMax, programReduceWithSwizzle, true);
            drawColorContrastStretch(textures[0], reductor.m_textures[0], 1);
            drawShiftHue(textures[1], 0);
            drawHSV2RGB(textures[0], 1);
            drawProtanopia(textures[1], 0);
            return  textures[0];
        case 7:
            drawGrayScale((GLuint) inTextureId, 0);
            drawGaussianBlur(1.0f, 3, textures[0], 1, 2);
            drawSobel(1.0f, (GLuint) inTextureId, 2, 1);
            return textures[1];
        case 8:
            drawGaussianBlur(1.0f, 3, (GLuint) inTextureId, 0, 1);
            drawUnsharpMask(1.0f, (GLuint) inTextureId, 1, 2);
            return textures[2];
        case 9:
            drawGrayScale((GLuint) inTextureId, 0);
            drawBinaryThreshold(0.3f, 0.0f, textures[0], 1);
            return textures[1];
        case 10:
            drawGrayScale((GLuint) inTextureId, 0);
            drawAdaptiveThreshold(3, 0.07f, 1.0f, textures[0], 1);
            return textures[1];
        case 11:
            // Grayscale contrast stretch
            drawGrayScale((GLuint) inTextureId, 0);
            reductor.Reduce(VAO,frameBuffers[0], textures[0],frameBufferWidth, frameBufferHeight,programFillTexture, programParallelReductionMinMax, programReduceWithSwizzle, false);
            drawContrastStretch(textures[0], reductor.m_textures[0], 1);
            return textures[1];
        case 12:
            // HSV color contrast stretch
            drawRGB2HSV((GLuint) inTextureId, 0);
            reductor.Reduce(VAO, frameBuffers[0], textures[0], frameBufferWidth, frameBufferHeight, programFillTexture, programParallelReductionMinMax, programReduceWithSwizzle, true);
            drawColorContrastStretch(textures[0], reductor.m_textures[0], 1);
            drawHSV2RGB(textures[1], 2);
            return textures[2];
    }
}

}


