set(src_dirs
            LCD
            LED
            SPI
            RTC)

set(include_dirs
            LCD
            LED
            SPI
            RTC)

set(requires
            driver
            newlib)

idf_component_register(SRC_DIRS ${src_dirs} INCLUDE_DIRS ${include_dirs} REQUIRES ${requires})

component_compile_options(-ffast-math -O3 -Wno-error=format=-Wno-format)