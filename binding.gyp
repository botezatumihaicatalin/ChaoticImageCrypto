{
  "targets": [
    {
      "target_name": "cryptography",
      "sources": [
        "cryptography.cxx"
      ],
      "include_dirs": [
        "<!(node -e \"require(\"nan\")\")"
      ],
      "cflags!": [
        "-fno-exceptions"
      ],
      "cflags_cc!": [
        "-fno-exceptions"
      ],
      "conditions": [
        [
          "OS==\"mac\"",
          {
            "xcode_settings": {
              "GCC_ENABLE_CPP_EXCEPTIONS": "YES",
              "GCC_ENABLE_CPP_RTTI": "YES"
            }
          }
        ],
        [
          "OS==\"win\"",
          {
            "configurations": {
              "Debug": {
                "msvs_settings": {
                  "VCCLCompilerTool": {
                    "RuntimeTypeInfo": "true"
                  }
                }
              },
              "Release": {
                "msvs_settings": {
                  "VCCLCompilerTool": {
                    "RuntimeTypeInfo": "true"
                  }
                }
              }
            }
          }
        ]
      ]
    }
  ]
}