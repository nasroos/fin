cmake_minimum_required(VERSION 3.10)
project(ClinicManagementSystem)

set(CMAKE_CXX_STANDARD 11)

# Locate necessary packages
find_package(Threads REQUIRED)

# Add executable for main.cpp
add_executable(clinic_management_system
        main.cpp
        ClinicManagementSystem.cpp
        Patient.cpp
        Appointment.cpp
        Billing.cpp
        Billing.cpp
        Billing.h)

# Link necessary libraries
target_link_libraries(clinic_management_system Threads::Threads)
