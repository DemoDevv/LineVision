For avoid problems with clang/clangd, you have to use compile_commands.json.  
You can generate it by running `bear -- make`.

This project uses make with gcc.
I Use for now the external library stb_image.h for image loading.

[ ] - decode EAN-8  
[ ] - decode EAN-13  
[ ] - decode UPC-A  

Specs:  
EAN-8 - http://www.gomaro.ch/Specifications/EAN8.htm
EAN-13 - http://www.gomaro.ch/Specifications/EAN13.htm
