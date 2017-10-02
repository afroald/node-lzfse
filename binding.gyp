{
  "targets": [
    {
      "target_name": "lzfse",
      "include_dirs": [
        "lzfse/src",
         "<!(node -e \"require('nan')\")"
      ],
      "sources": [
          "src/lzfse-binding.cc",
          "lzfse/src/lzfse_decode.c",
          "lzfse/src/lzfse_decode_base.c",
          "lzfse/src/lzfse_encode.c",
          "lzfse/src/lzfse_encode_base.c",
          "lzfse/src/lzfse_fse.c",
          "lzfse/src/lzvn_decode_base.c",
          "lzfse/src/lzvn_encode_base.c"
      ]
    }
  ]
}
