idf_component_register(SRCS "ldr.c"
                       INCLUDE_DIRS "include"
                       REQUIRES esp_adc)

# IDF sets -Wno-unused-parameter globally. Re-enable it: an unused parameter
# is the most common symptom of forgetting to read a struct field.
target_compile_options(${COMPONENT_LIB} PRIVATE -Wextra -Wunused-parameter)
