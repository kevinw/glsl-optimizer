#include <stdio.h>
#include <cstring>
#include <cassert>
#include "glsl_optimizer.h"

extern "C" {
    bool optimize(const char* shaderSource, glslopt_shader_type shaderType, char* output, int outputSize) {
        auto ctx = glslopt_initialize(kGlslTargetOpenGL);
        const unsigned options = kGlslOptionSkipPreprocessor;
        bool res;
        auto shader = glslopt_optimize (ctx, shaderType, shaderSource, options);
        if (glslopt_get_status (shader)) {
            auto shaderOutput = glslopt_get_output (shader);
            if (strlen(output) < outputSize) {
                strncpy(output, shaderOutput, outputSize);
                res = true;
            } else {
                strncpy(output, "output buf too small", outputSize);
                res = false;
            }
        } else {
            auto errorLog = glslopt_get_log (shader);
            assert(strlen(errorLog) < outputSize);
            strncpy(output, errorLog, outputSize);
            res = false;
        }
        glslopt_shader_delete (shader);
        glslopt_cleanup (ctx);
        return res;
    }
}
