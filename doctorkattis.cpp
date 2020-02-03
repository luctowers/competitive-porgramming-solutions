// https://open.kattis.com/problems/doctorkattis

#include <iostream>
#include <list>
#include <map>
#include <unordered_map>

class Clinic {

private:

  struct Patient {
    unsigned arrival_index; // indicates the order in which patients arrived
    unsigned severity; // indicates severity of patients injury
    std::string name; // eg. FOO, BAT
    unsigned long long priority() {
      // zero represents the highest priority
      // priority is a 64-bit int composed of the severity and arrival_index of the patient
      // the most significant 32 bits are the inverted severity
      // the least significant 32 bits are the arrival_index
      // this means patients with a higher severity will have a priority closer to zero
      // but if severity is equal the patient with the lesser arrival_index will have a priority closer to zero
      return ((unsigned long long)(0xFFFFFFFF-severity) << 32) | arrival_index;
    }
  };

  unsigned patient_count; // used to assign arrival_index to patients
  std::list<Patient> patient_list; // stores untreated patients
  std::unordered_map<std::string,std::list<Patient>::iterator> patient_name_map; // provides an index by name
  std::map<unsigned long long,std::list<Patient>::iterator> patient_priority_map; // provides an index by piority

public:

  Clinic() {
    patient_count = 0;
  }

  void addPatient(const std::string& name, unsigned severity) {
    // create patient
    patient_list.emplace_front();
    auto patient = patient_list.begin();

    // set patient data
    patient->arrival_index = patient_count++;
    patient->name = name;
    patient->severity = severity;

    // make index entries
    patient_name_map[name] = patient;
    patient_priority_map[patient->priority()] = patient;
  }

  void updatePatient(const std::string& name, unsigned severityIncrease) {
    auto patient = patient_name_map[name];
    patient_priority_map.erase(patient->priority()); // erase old index entry
    patient->severity += severityIncrease;
    patient_priority_map[patient->priority()] = patient; // create new index entry
  }

  void treatPatient(const std::string& name) {
    auto patient = patient_name_map[name];

    // remove index entries
    patient_priority_map.erase(patient->priority());
    patient_name_map.erase(patient->name);

    // destroy patient
    patient_list.erase(patient);
  }

  const Patient* queryPatient() {
    if (patient_priority_map.empty())
      return NULL;
    else
      return &*patient_priority_map.begin()->second;
  }

};

int main() {
  unsigned command_count, command_id;
  std::string patient_name;
  Clinic clinic;

  std::cin >> command_count;
  for (unsigned i = 0; i < command_count; i++) {
    std::cin >> command_id;

    // commands 0 though 2 all have patient_name as the first argument
    if (command_id <= 2)
      std::cin >> patient_name;

    if (command_id == 0) {
      unsigned patient_severity;
      std::cin >> patient_severity;
      clinic.addPatient(patient_name, patient_severity);
    }
    else if (command_id == 1) {
      unsigned patient_severity_increase;
      std::cin >> patient_severity_increase;
      clinic.updatePatient(patient_name, patient_severity_increase);
    }
    else if (command_id == 2) {
      clinic.treatPatient(patient_name);
    }
    else if (command_id == 3) {
      auto patient = clinic.queryPatient();
      if (patient)
        std::cout << patient->name << std::endl;
      else
        std::cout << "The clinic is empty" << std::endl;
    }
    
  }

  return 0;
}
