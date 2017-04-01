{
 "targets": [ 
   { 
     "target_name": "cryptography",
     "sources": [ "cryptography.cxx" ],
     "include_dirs": [ "<!(node -e \"require('nan')\")" ]
   } 
 ]
}