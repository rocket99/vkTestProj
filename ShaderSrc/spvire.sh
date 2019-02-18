#!shell

glslangValidator -V vert_fst.vert   -o ../spv/vert_fst.spv
glslangValidator -V frag_fst.frag   -o ../spv/frag_fst.spv 

glslangValidator -V basic_frag.frag -o ../spv/basic_frag.spv
glslangValidator -V basic_vert.vert -o ../spv/basic_vert.spv

glslangValidator -V light_frag.frag -o ../spv/light_frag.spv
glslangValidator -V light_vert.vert -o ../spv/light_vert.spv
