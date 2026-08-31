# IDF 5.3+ split the monolithic `driver` component; take only what we need.
idf_component_register(SRCS "relay.c"
                       INCLUDE_DIRS "include"
                       PRIV_REQUIRES esp_driver_gpio soc)

# IDF sets -Wno-unused-parameter globally. Re-enable it: an unused parameter
# is the most common symptom of forgetting to read a struct field.
target_compile_options(${COMPONENT_LIB} PRIVATE -Wextra -Wunused-parameter)
