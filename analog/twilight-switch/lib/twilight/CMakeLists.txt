idf_component_register(SRCS "twilight.c"
                       INCLUDE_DIRS "include")

# IDF sets -Wno-unused-parameter globally. Re-enable it: an unused parameter
# is the most common symptom of forgetting to read a struct field.
target_compile_options(${COMPONENT_LIB} PRIVATE -Wextra -Wunused-parameter)
