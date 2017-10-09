#include <node.h>
#include "lzfse.h"
#include "lzfse-binding.h"

namespace nodelzfse {

using namespace v8;
using namespace node;

// Same as realloc(x,s), except x is freed when realloc fails
static inline void *lzfse_reallocf(void *x, size_t s) {
    void *y = realloc(x, s);
    if (y == 0) {
      free(x);
      return 0;
    }
    return y;
}

enum { LZFSE_ENCODE = 0, LZFSE_DECODE };

void Lzfse(int op, Local<Object>& input_buffer, Local<Object>& output_buffer) {
    size_t input_size = Buffer::Length(input_buffer);

    if (input_size == 0) {
        Nan::ThrowRangeError("Input is empty");
        return;
    }

    if (op == LZFSE_DECODE && input_size > SIZE_MAX / 4) {
        Nan::ThrowRangeError("Input is too large");
        return;
    }

    uint8_t *input = (uint8_t *)Buffer::Data(input_buffer);

    size_t output_allocated = (op == LZFSE_ENCODE) ? input_size : (4 * input_size);
    size_t output_size = 0;
    size_t aux_allocated = (op == LZFSE_ENCODE) ? lzfse_encode_scratch_size()
                                                : lzfse_decode_scratch_size();

    void *aux = aux_allocated ? malloc(aux_allocated) : 0;
    if (aux_allocated != 0 && aux == 0) {
        Nan::ThrowError("Failed to allocate memory");
        return;
    }

    uint8_t *output = (uint8_t *)malloc(output_allocated);
    if (output == 0) {
        Nan::ThrowError("Failed to allocate memory");
        return;
    }

    while (1) {
        if (op == LZFSE_ENCODE)
            output_size = lzfse_encode_buffer(output, output_allocated, input, input_size, aux);
        else
            output_size = lzfse_decode_buffer(output, output_allocated, input, input_size, aux);

        if (output_size == 0 || (op == LZFSE_DECODE && output_size == output_allocated)) {
            if (output_allocated > SIZE_MAX >> 1) {
                Nan::ThrowError("Not enough memory");
                return;
            }

            output_allocated <<= 1;
            output = (uint8_t *)lzfse_reallocf(output, output_allocated);

            if (output == 0) {
                Nan::ThrowError("Failed to allocate memory");
                return;
            }

            continue;
        }

        break;
    }

    output_buffer = Nan::NewBuffer((char*)output, output_size).ToLocalChecked();

    free(aux);
}

NAN_METHOD(CompressBinding::Sync) {
    if (!Buffer::HasInstance(info[0])) {
        Nan::ThrowTypeError("Input must be a buffer");
        return;
    }

    Local<Object> input_buffer = info[0]->ToObject();
    Local<Object> output_buffer;

    Lzfse(LZFSE_ENCODE, input_buffer, output_buffer);

    info.GetReturnValue().Set(output_buffer);
}

NAN_METHOD(DecompressBinding::Sync) {
    if (!Buffer::HasInstance(info[0])) {
        Nan::ThrowTypeError("Input must be a buffer");
        return;
    }

    Local<Object> input_buffer = info[0]->ToObject();
    Local<Object> output_buffer;

    Lzfse(LZFSE_DECODE, input_buffer, output_buffer);

    info.GetReturnValue().Set(output_buffer);
}

NAN_MODULE_INIT(Init) {
    Nan::SetMethod(target, "compressSync", CompressBinding::Sync);
    Nan::SetMethod(target, "decompressSync", DecompressBinding::Sync);
}

NODE_MODULE(lzfse, Init)

}  // namespace nodelzfse
