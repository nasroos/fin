#include "ClinicManagementSystem.h"
#include <iostream>
#include <cstring>
#include <unistd.h>

ClinicManagementSystem::ClinicManagementSystem() :
        registrationPipe{-1, -1},
        schedulingPipe{-1, -1},
        billingPipe{-1, -1} {
    // Create pipes
    if (pipe(registrationPipe) < 0 || pipe(schedulingPipe) < 0 || pipe(billingPipe) < 0) {
        std::cerr << "Error: Unable to create pipes.\n";
        return;
    }

    // Start threads
    registrationThread = std::thread(&ClinicManagementSystem::registrationThreadFunction, this);
    schedulingThread = std::thread(&ClinicManagementSystem::schedulingThreadFunction, this);
    billingThread = std::thread(&ClinicManagementSystem::billingThreadFunction, this);
}

ClinicManagementSystem::~ClinicManagementSystem() {
    // Join threads
    registrationThread.join();
    schedulingThread.join();
    billingThread.join();

    // Close pipes
    close(registrationPipe[0]);
    close(registrationPipe[1]);
    close(schedulingPipe[0]);
    close(schedulingPipe[1]);
    close(billingPipe[0]);
    close(billingPipe[1]);
}

void ClinicManagementSystem::start() {
    int choice;

    do {
        // Display menu
        std::cout << "\nClinic Management System Menu\n";
        std::cout << "1. Register Patient\n";
        std::cout << "2. Schedule Appointment\n";
        std::cout << "3. Generate Bill\n";
        std::cout << "4. Display Patient\n";
        std::cout << "5. Display Scheduled Appointments\n";
        std::cout << "6. Display Generated Bills\n";
        std::cout << "7. Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
            case 1:
                handleRegistration();
                break;
            case 2:
                handleScheduling();
                break;
            case 3:
                handleBilling();
                break;
            case 4:
                displayPatient();
                break;
            case 5:
                displayAppointments();
                break;
            case 6:
                displayBills();
                break;
            case 7:
                std::cout << "Exiting...\n";
                break;
            default:
                std::cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 7);
}

void ClinicManagementSystem::handleRegistration() {
    Patient patient;
    std::cout << "\nEnter patient details:\n";
    std::cout << "Name: ";
    std::cin >> patient.name;
    std::cout << "Age: ";
    std::cin >> patient.age;
    std::cout << "Gender: ";
    std::cin >> patient.gender;
    std::string data = patient.name + "," + std::to_string(patient.age) + "," + patient.gender;
    write(registrationPipe[1], data.c_str(), data.size());
}

void ClinicManagementSystem::handleScheduling() {
    Appointment appointment;
    std::cout << "\nEnter appointment details:\n";
    std::cout << "Patient Name: ";
    std::cin >> appointment.patientName;
    std::cout << "Doctor Name: ";
    std::cin >> appointment.doctorName;
    std::cout << "Appointment Time (Unix Timestamp): ";
    std::cin >> appointment.appointmentTime;
    std::string data = appointment.patientName + "," + appointment.doctorName + "," + std::to_string(appointment.appointmentTime);
    write(schedulingPipe[1], data.c_str(), data.size());
}

void ClinicManagementSystem::handleBilling() {
    Billing bill;
    std::cout << "\nEnter billing details:\n";
    std::cout << "Patient Name: ";
    std::cin >> bill.patientName;
    std::cout << "Amount: ";
    std::cin >> bill.amount;
    std::string data = bill.patientName + "," + std::to_string(bill.amount);
    write(billingPipe[1], data.c_str(), data.size());
}

void ClinicManagementSystem::displayPatient() {
    char buffer[256];
    ssize_t bytesRead = read(registrationPipe[0], buffer, sizeof(buffer) - 1);
    buffer[bytesRead] = '\0';
    std::string data(buffer);
    std::cout << "\nPatient Information:\n";
    std::cout << data << std::endl;
}

void ClinicManagementSystem::displayAppointments() {
    char buffer[256];
    ssize_t bytesRead = read(schedulingPipe[0], buffer, sizeof(buffer) - 1);
    buffer[bytesRead] = '\0';
    std::string data(buffer);
    std::cout << "\nScheduled Appointment Information:\n";
    std::cout << data << std::endl;
}

void ClinicManagementSystem::displayBills() {
    char buffer[256];
    ssize_t bytesRead = read(billingPipe[0], buffer, sizeof(buffer) - 1);
    buffer[bytesRead] = '\0';
    std::string data(buffer);
    std::cout << "\nGenerated Bill Information:\n";
    std::cout << data << std::endl;
}

void ClinicManagementSystem::registrationThreadFunction() {
    while (true) {
        char buffer[256];
        ssize_t bytesRead = read(registrationPipe[0], buffer, sizeof(buffer) - 1);
        if (bytesRead > 0) {
            buffer[bytesRead] = '\0';
            std::string data(buffer);
            std::cout << "\nPatient Information:\n";
            std::cout << data << std::endl;
        }
    }
}

void ClinicManagementSystem::schedulingThreadFunction() {
    while (true) {
        char buffer[256];
        ssize_t bytesRead = read(schedulingPipe[0], buffer, sizeof(buffer) - 1);
        if (bytesRead > 0) {
            buffer[bytesRead] = '\0';
            std::string data(buffer);
            std::cout << "\nScheduled Appointment Information:\n";
            std::cout << data << std::endl;
        }
    }
}

void ClinicManagementSystem::billingThreadFunction() {
    while (true) {
        char buffer[256];
        ssize_t bytesRead = read(billingPipe[0], buffer, sizeof(buffer) - 1);
        if (bytesRead > 0) {
            buffer[bytesRead] = '\0';
            std::string data(buffer);
            std::cout << "\nGenerated Bill Information:\n";
            std::cout << data << std::endl;
        }
    }
}
