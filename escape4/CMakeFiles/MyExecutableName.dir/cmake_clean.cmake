FILE(REMOVE_RECURSE
  "CMakeFiles/MyExecutableName.dir/StaticScene.cpp.o"
  "bin/MyExecutableName.pdb"
  "bin/MyExecutableName"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang CXX)
  INCLUDE(CMakeFiles/MyExecutableName.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
