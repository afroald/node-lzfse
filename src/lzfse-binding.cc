#include <node.h>
#include "lzfse.h"
#include "lzfse-binding.h"

namespace nodelzfse {

using namespace v8;
using namespace node;

NAN_METHOD(CompressBinding::Sync) {
    Local<Object> in_buffer = info[0]->ToObject();
    size_t in_size = Buffer::Length(in_buffer);

    if (in_size == 0) {
        Nan::ThrowRangeError("Input is empty");
    }

    uint8_t *in = (uint8_t *)Buffer::Data(in_buffer);
    size_t out_allocated = in_size;
    size_t out_size = 0;
    size_t aux_allocated = lzfse_encode_scratch_size();
    void *aux = aux_allocated ? malloc(aux_allocated) : 0;

    uint8_t *out = (uint8_t *)malloc(out_allocated);

    out_size = lzfse_encode_buffer(out, out_allocated, in, in_size, aux);

    info.GetReturnValue()
        .Set(Nan::NewBuffer((char*)out, out_size).ToLocalChecked());

    // free(in);
    // free(out);
    free(aux);
}

NAN_METHOD(DecompressBinding::Sync) {
    Local<Object> in_buffer = info[0]->ToObject();
    size_t in_size = Buffer::Length(in_buffer);

    if (in_size * 4 > SIZE_MAX) {
        Nan::ThrowRangeError("Input is too large");
    }

    if (in_size == 0) {
        Nan::ThrowRangeError("Input is empty");
    }

    uint8_t *in = (uint8_t *)Buffer::Data(in_buffer);
    size_t out_allocated = (4 * in_size);
    size_t out_size = 0;
    size_t aux_allocated = lzfse_decode_scratch_size();
    void *aux = aux_allocated ? malloc(aux_allocated) : 0;

    uint8_t *out = (uint8_t *)malloc(out_allocated);

    out_size = lzfse_decode_buffer(out, out_allocated, in, in_size, aux);

    info.GetReturnValue()
        .Set(Nan::NewBuffer((char*)out, out_size).ToLocalChecked());
    
    // free(in);
    // free(out);
    free(aux);
}

NAN_MODULE_INIT(Init) {
    Nan::SetMethod(target, "compressSync", CompressBinding::Sync);
    Nan::SetMethod(target, "decompressSync", DecompressBinding::Sync);
}

NODE_MODULE(lzfse, Init)

}  // namespace nodelzfse
