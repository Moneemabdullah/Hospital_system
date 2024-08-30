/*
    MD Abdullah al moneem

    Institute:- BAIUST
    Country:- BANGLADESH
*/

#include <bits/stdc++.h>
using namespace std;

class Patient
{
public:
    string id;
    string name;
    int age;
    string medicalHistory;
    double totalCost;
    string doctor_id;

    void add_cost(double cost)
    {
        totalCost += cost;
    }
};

class Doctor
{
public:
    string id;
    string name;
    string specialization;
    double feesPerAppointment;
    double totalEarnings;
    vector<Patient> appointments;

    void add_appointment(const Patient &patient)
    {
        appointments.push_back(patient);
    }

    void add_earnings(double amount)
    {
        totalEarnings += amount;
    }

    double total_earnings() const
    {
        return totalEarnings;
    }
};

class Hospital
{
public:
    map<string, vector<Doctor>> Doctor_database;
    map<string, vector<Patient>> Patient_database;

    void read_Doctors_data()
    {
        ifstream file("doctors.txt");
        if (!file.is_open())
        {
            cout << "\tError opening file: doctors.txt" << endl;
            return;
        }

        string line;
        while (getline(file, line))
        {
            if (line.empty())
                continue;

            string id = line;

            if (!getline(file, line))
                continue;
            string name = line;

            if (!getline(file, line))
                continue;
            string specialization = line;

            if (!getline(file, line))
                continue;
            double feesPerAppointment;
            stringstream(line) >> feesPerAppointment;

            Doctor doctor;
            doctor.id = id;
            doctor.name = name;
            doctor.specialization = specialization;
            doctor.feesPerAppointment = feesPerAppointment;

            Doctor_database[specialization].push_back(doctor);
        }

        file.close();
    }

    void read_Patients_data()
    {
        ifstream file("patients.txt");
        if (!file.is_open())
        {
            cout << "\tError opening file: patients.txt" << endl;
            return;
        }

        string line;
        while (getline(file, line))
        {
            if (line.empty())
                continue;

            string id = line;

            if (!getline(file, line))
                continue;
            string name = line;

            int age;
            if (!(file >> age))
                continue;
            file.ignore();

            string medicalHistory;
            if (!getline(file, line))
                continue;
            medicalHistory = line;

            double totalCost;
            if (!(file >> totalCost))
                continue;
            file.ignore();

            string doctor_id;
            if (!getline(file, line))
                continue;
            doctor_id = line;

            Patient patient;
            patient.id = id;
            patient.name = name;
            patient.age = age;
            patient.medicalHistory = medicalHistory;
            patient.totalCost = totalCost;
            patient.doctor_id = doctor_id;

            Patient_database[doctor_id].push_back(patient);
        }

        file.close();
    }

    vector<Doctor> display_available_doctors(const string &specialization)
    {
        if (Doctor_database.find(specialization) == Doctor_database.end())
        {
            cout << "\tNo doctors found with specialization: " << specialization << endl;
            return {};
        }

        vector<Doctor> &doctors = Doctor_database[specialization];

        return doctors;
    }
};

class Patients_part : public Hospital
{
public:
    void confirm_appointment()
    {
        cout << "\t=======================================================" << endl;
        cout << "\t\tAvailable Specializations:" << endl
             << endl;
        cout << "\t\t1. Cardiologist" << endl;
        cout << "\t\t2. Neurologist" << endl;
        cout << "\t\t3. Dermatologist" << endl;
        cout << "\t\t4. Oncologist" << endl;
        cout << "\t\t5. Urologist" << endl;
        cout << "\t\t6. Gastroenterologist" << endl;
        cout << "\t=======================================================" << endl
             << endl;

        cout << "\tEnter your specialization choice (1-6): ";
        int choice;
        cin >> choice;

        string specialization;
        switch (choice)
        {
        case 1:
            specialization = "Cardiologist";
            break;
        case 2:
            specialization = "Neurologist";
            break;
        case 3:
            specialization = "Dermatologist";
            break;
        case 4:
            specialization = "Oncologist";
            break;
        case 5:
            specialization = "Urologist";
            break;
        case 6:
            specialization = "Gastroenterologist";
            break;
        default:
            cout << "\t\tInvalid choice. Please enter a number between 1 and 6." << endl;
            return;
        }

        if (Doctor_database.find(specialization) == Doctor_database.end())
        {
            cout << "\t\tNo doctors found with " << specialization << endl;
            return;
        }

        vector<Doctor> &availableDoctors = Doctor_database[specialization];
        cout << "\t=======================================================" << endl;
        cout << "\t\tAvailable Doctors for " << specialization << " specialization:" << endl;
        int s = 1;
        for (const auto &doctor : availableDoctors)
        {
            cout << "\t " << s++ << ". " << endl;
            cout << "\t  Doctor ID: " << doctor.id << endl;
            cout << "\t  Name: " << doctor.name << endl;
            cout << "\t  Fees per Appointment: $" << doctor.feesPerAppointment << endl;
            cout << "\t-----------------------------------" << endl;
        }

        cout << "\tEnter the Doctor choice you want to book: ";
        int doctor_choice;
        cin >> doctor_choice;

        if (doctor_choice < 1 || doctor_choice > availableDoctors.size())
        {
            cout << "\tInvalid doctor choice. Please enter a number between 1 and " << availableDoctors.size() << "." << endl;
            return;
        }

        Doctor &selectedDoctor = availableDoctors[doctor_choice - 1];

        string patient_id;
        cout << "\tEnter Patient ID: ";
        cin >> patient_id;

        Patient patient;
        bool patient_found = false;
        for (auto &pair : Patient_database)
        {
            for (auto &p : pair.second)
            {
                if (p.id == patient_id)
                {
                    patient = p;
                    patient_found = true;
                    break;
                }
            }
            if (patient_found)
                break;
        }

        if (!patient_found)
        {
            cout << "\tPatient with ID: " << patient_id << " not found." << endl;
            return;
        }

        double appointmentCost = selectedDoctor.feesPerAppointment;
        patient.add_cost(appointmentCost);

        selectedDoctor.add_earnings(appointmentCost);
        selectedDoctor.add_appointment(patient);

        for (auto &doctor : Doctor_database[specialization])
        {
            if (doctor.id == selectedDoctor.id)
            {
                doctor = selectedDoctor;
                break;
            }
        }

        for (auto &pair : Patient_database)
        {
            for (auto &p : pair.second)
            {
                if (p.id == patient.id)
                {
                    p = patient;
                    break;
                }
            }
        }

        cout << "\t=======================================================" << endl;
        cout << "\t\tAppointment confirmed!" << endl
             << endl;
        cout << "\t\tPatient Name: " << patient.name << endl;
        cout << "\t\tDoctor Name: " << selectedDoctor.name << endl;
        cout << "\t\tAppointment Fee: $" << appointmentCost << endl
             << endl;
        cout << "\t=======================================================" << endl;
    }

    void register_patient()
    {
        int newPatientID = 1;
        for (const auto &pair : Patient_database)
        {
            newPatientID += pair.second.size();
        }
        string id = "P" + to_string(newPatientID);

        cout << "\t=======================================================" << endl;

        cout << "\tEnter Patient Name: ";
        string name;
        cin.ignore();
        getline(cin, name);

        cout << "\tEnter Patient Age: ";
        int age;
        cin >> age;

        string medicalHistory = "";
        double totalCost = 0.0;
        string doctor_id = "";

        Patient newPatient;
        newPatient.id = id;
        newPatient.name = name;
        newPatient.age = age;
        newPatient.medicalHistory = medicalHistory;
        newPatient.totalCost = totalCost;
        newPatient.doctor_id = doctor_id;

        Patient_database[doctor_id].push_back(newPatient);

        cout << "\t=======================================================" << endl;
        cout << "\tPatient registered successfully!" << endl;
        cout << "\t\tPatient ID: " << id << endl;
        cout << "\t\tPatient Name: " << name << endl;
        cout << "\t=======================================================" << endl;
    }
};

class Doctors_part : public Hospital
{
public:
    void view_patients(const string &doc_id)
    {
        cout << "\t=======================================================" << endl;
        cout << "\t\tPatients for Doctor ID: " << doc_id << endl;
        bool found = false;
        for (const auto &pair : Patient_database)
        {
            for (const auto &patient : pair.second)
            {
                if (doc_id == patient.doctor_id)
                {
                    cout << "\tPatient ID: " << patient.id << ", Name: " << patient.name << ", Age: " << patient.age << endl;
                    found = true;
                }
            }
        }

        if (!found)
        {
            cout << "\tNo patients found for Doctor ID: " << doc_id << endl;
        }
        cout << "\t=======================================================" << endl;
    }

    void view_total_earnings(const string &doctor_id)
    {
        cout << "\t=======================================================" << endl;
        bool found = false;
        for (const auto &pair : Doctor_database)
        {
            for (const auto &doctor : pair.second)
            {
                if (doctor.id == doctor_id)
                {
                    double earnings = doctor.total_earnings();
                    cout << "\t\tTotal earnings for Doctor: " << doctor.name << " (ID: " << doctor.id << "): $" << earnings << endl;
                    found = true;
                    break;
                }
            }
            if (found)
                break;
        }

        if (!found)
        {
            cout << "\t\tDoctor with ID: " << doctor_id << " not found." << endl;
        }
        cout << "\t=======================================================" << endl;
    }
};

class Management : public Hospital
{
public:
    void all_doctors_information()
    {
        cout << "\t=======================================================" << endl;
        cout << "\t\tDoctor Database Contents:" << endl;
        for (const auto &pair : Doctor_database)
        {
            cout << "\t\tSpecialization: " << pair.first << endl;
            for (const auto &doctor : pair.second)
            {
                cout << "\t\t  Doctor ID: " << doctor.id << ", Name: " << doctor.name << ", Fees: $" << doctor.feesPerAppointment << endl;
            }
            cout << "\t\t-----------------------------------" << endl;
        }
        cout << "\t=======================================================" << endl;
    }

    void all_patient_information()
    {
        cout << "\t=======================================================" << endl;
        cout << "\t\tPatient Database Contents:" << endl;
        for (const auto &pair : Patient_database)
        {
            for (const auto &patient : pair.second)
            {
                cout << "\t\t  Patient ID: " << patient.id << ", Name: " << patient.name << ", Age: " << patient.age << endl;
            }
            cout << "\t\t-----------------------------------" << endl;
        }
        cout << "\t=======================================================" << endl;
    }

    void revenue_calculation()
    {
        double totalRevenue = 0.0;
        double totalManagementEarnings = 0.0;
        double totalDoctorEarnings = 0.0;

        for (const auto &pair : Doctor_database)
        {
            for (const auto &doctor : pair.second)
            {
                double doctorEarnings = doctor.total_earnings();
                totalRevenue += doctorEarnings;
            }
        }

        totalManagementEarnings = 0.15 * totalRevenue;
        totalDoctorEarnings = 0.85 * totalRevenue;

        cout << "\t=======================================================" << endl;
        cout << "\t\tRevenue Calculation:" << endl
             << endl;
        cout << "\tTotal Revenue: $" << totalRevenue << endl;
        cout << "\tTotal Earnings for Management: $" << totalManagementEarnings << endl;
        cout << "\tTotal Earnings for Doctors: $" << totalDoctorEarnings << endl;
        cout << "\t=======================================================" << endl;
    }
};

int main()
{
    Patients_part patientsPart;
    Doctors_part doctorsPart;
    Management management;

    patientsPart.read_Doctors_data();
    patientsPart.read_Patients_data();

    doctorsPart.read_Doctors_data();
    doctorsPart.read_Patients_data();

    management.read_Doctors_data();
    management.read_Patients_data();

    cout << "\t=================================================================" << endl;

    cout << "\t     #   #  #####   ####  #####  ###  #######  ####   #         " << endl;
    cout << "\t     #   #  #   #  #      #   #   #      #    #    #  #         " << endl;
    cout << "\t     #####  #   #   ###   #####   #      #    ######  #         " << endl;
    cout << "\t     #   #  #   #      #  #       #      #    #    #  #         " << endl;
    cout << "\t     #   #  #####  ####   #      ###     #    #    #  ######    " << endl;

    cout << "\t=================================================================" << endl;
    cout << "\t\tWelcome to the Hospital Management System" << endl;

    while (true)
    {

        cout << "\t\t1. Patient" << endl;
        cout << "\t\t2. Doctor" << endl;
        cout << "\t\t3. Management" << endl
             << endl;
        cout << "\tEnter your choice (1/2/3): ";
        int choice;
        cin >> choice;

        switch (choice)
        {
        case 1:
        {
            while (true)
            {
                cout << "\t=======================================================" << endl;
                cout << "\t\tPatient Menu" << endl;
                cout << "\t\t1. Book an Appointment" << endl;
                cout << "\t\t2. Register New Patient" << endl;
                cout << "\t\t3. Return to Main Menu" << endl
                     << endl;
                cout << "\tEnter your choice: ";
                int patient_choice;
                cin >> patient_choice;

                switch (patient_choice)
                {
                case 1:
                    patientsPart.confirm_appointment();
                    break;
                case 2:
                    patientsPart.register_patient();
                    break;
                case 3:
                    break;
                default:
                    cout << "\tInvalid choice. Please enter 1, 2, or 3." << endl;
                    break;
                }
                if (patient_choice == 3)
                    break;
            }
            break;
        }
        case 2:
        {
            cout << "\tEnter Doctor ID: ";
            string doctor_id;
            cin >> doctor_id;
            string pass;
            cout << "\tEnter password (1234): ";
            cin >> pass;
            if (pass != "1234")
            {
                cout << "\tPassword is incorrect." << endl;
                break;
            }

            while (true)
            {
                cout << "\t=======================================================" << endl;
                cout << "\t\tDoctor Menu" << endl;
                cout << "\t\t1. View Patients" << endl;
                cout << "\t\t2. View Total Earnings" << endl;
                cout << "\t\t3. Return to Main Menu" << endl
                     << endl;
                cout << "\tEnter your choice: ";
                int doctor_choice;
                cin >> doctor_choice;

                switch (doctor_choice)
                {
                case 1:
                    doctorsPart.view_patients(doctor_id);
                    break;
                case 2:
                    doctorsPart.view_total_earnings(doctor_id);
                    break;
                case 3:
                    break;
                default:
                    cout << "\tInvalid choice. Please enter 1, 2, or 3." << endl;
                    break;
                }
                if (doctor_choice == 3)
                    break;
            }
            break;
        }
        case 3:
        {
            string pass;
            cout << "\tEnter password (1234): ";
            cin >> pass;
            if (pass != "1234")
            {
                cout << "\tPassword is incorrect." << endl;
                break;
            }

            while (true)
            {
                cout << "\t=======================================================" << endl;
                cout << "\t\tManagement Menu" << endl;
                cout << "\t\t1. View All Doctors" << endl;
                cout << "\t\t2. View All Patients" << endl;
                cout << "\t\t3. Calculate Total Revenue" << endl;
                cout << "\t\t4. Return to Main Menu" << endl
                     << endl;
                cout << "\tEnter your choice: ";
                int management_choice;
                cin >> management_choice;

                switch (management_choice)
                {
                case 1:
                    management.all_doctors_information();
                    break;
                case 2:
                    management.all_patient_information();
                    break;
                case 3:
                    management.revenue_calculation();
                    break;
                case 4:
                    break;
                default:
                    cout << "\tInvalid choice. Please enter 1, 2, 3, or 4." << endl;
                    break;
                }
                if (management_choice == 4)
                    break;
            }
            break;
        }
        default:
            cout << "\tInvalid choice. Please enter 1, 2, or 3." << endl;
            break;
        }

        cout << "\tDo you want to continue? (y/n): ";
        char continue_choice;
        cin >> continue_choice;
        if (continue_choice != 'y' && continue_choice != 'Y')
        {
            break;
        }
    }

    return 0;
}

/* Thank you */
