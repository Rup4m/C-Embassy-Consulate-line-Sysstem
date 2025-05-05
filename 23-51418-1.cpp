#include <iostream>
#include <string>
#include <iomanip>
using namespace std;


struct Applicant {
    string visaType;
    int tokenNumber;
};


const int MAX_APPLICANTS = 100;
const int MAX_PER_VISA = 25;
const int COUNTERS = 4;

// Arrays for applicants by visa type
Applicant touristQueue[MAX_PER_VISA];
Applicant medicalQueue[MAX_PER_VISA];
Applicant businessQueue[MAX_PER_VISA];
Applicant governmentQueue[MAX_PER_VISA];

// Counters for number of applicants in each queue
int touristCount = 0;
int medicalCount = 0;
int businessCount = 0;
int governmentCount = 0;

// Array to store how many served today
int visaServed[4] = {0, 0, 0, 0}; 


Applicant counterServed[COUNTERS][MAX_APPLICANTS];
int counterServedCount[COUNTERS] = {0, 0, 0, 0};


int globalToken = 1;
int totalApplicants = 0;

// Function to get visa short form
string getShortForm(const string& visaTypeFull) {
    if (visaTypeFull == "Tourist Visa") return "TR";
    if (visaTypeFull == "Medical Visa") return "MED";
    if (visaTypeFull == "Business Visa") return "BS";
    if (visaTypeFull == "Government Officials Visa") return "GO";
    return "";
}

// Function to get full form
string getFullForm(const string& shortForm) {
    if (shortForm == "TR") return "Tourist Visa";
    if (shortForm == "MED") return "Medical Visa";
    if (shortForm == "BS") return "Business Visa";
    if (shortForm == "GO") return "Government Officials Visa";
    return "";
}

// Function to print token box
void printTokenBox(string shortForm, int tokenNumber) {
    string token = shortForm + "-" + to_string(tokenNumber);
    int len = token.length();
    cout << "\n┌" << string(len + 4, '─') << "┐\n";
    cout << "│  " << token << "  │\n";
    cout << "└" << string(len + 4, '─') << "┘\n";
}

// Function to request token
void requestToken() {
    if (totalApplicants >= MAX_APPLICANTS) {
        cout << "\n🚫 Daily applicant limit reached!\n";
        return;
    }

    cout << "Enter Visa Type (Tourist Visa / Medical Visa / Business Visa / Government Officials Visa): ";
    cin.ignore();
    string visaTypeFull;
    getline(cin, visaTypeFull);

    string shortForm = getShortForm(visaTypeFull);
    if (shortForm == "") {
        cout << "\n🚫 Invalid visa type entered.\n";
        return;
    }

    //to Check visa specific limit
    if ((shortForm == "TR" && touristCount >= MAX_PER_VISA) ||
        (shortForm == "MED" && medicalCount >= MAX_PER_VISA) ||
        (shortForm == "BS" && businessCount >= MAX_PER_VISA) ||
        (shortForm == "GO" && governmentCount >= MAX_PER_VISA)) {
        cout << "\n🚫 " << visaTypeFull << " limit reached for today.\n";
        return;
    }

    Applicant applicant = {shortForm, globalToken};

    
    if (shortForm == "TR") touristQueue[touristCount++] = applicant;
    else if (shortForm == "MED") medicalQueue[medicalCount++] = applicant;
    else if (shortForm == "BS") businessQueue[businessCount++] = applicant;
    else if (shortForm == "GO") governmentQueue[governmentCount++] = applicant;

    printTokenBox(shortForm, globalToken);

    globalToken++;
    totalApplicants++;
}

// Function  primary visa for a counter
string getPrimaryVisa(int counterNumber) {
    if (counterNumber == 1) return "TR";
    if (counterNumber == 2) return "MED";
    if (counterNumber == 3) return "BS";
    if (counterNumber == 4) return "GO";
    return "";
}


bool servePrimary(int counterNumber, string visaType) {
    Applicant applicant;
    bool served = false;

    if (visaType == "TR" && touristCount > 0) {
        applicant = touristQueue[0];
        for (int i = 0; i < touristCount - 1; ++i) touristQueue[i] = touristQueue[i+1];
        touristCount--;
        served = true;
    }
    else if (visaType == "MED" && medicalCount > 0) {
        applicant = medicalQueue[0];
        for (int i = 0; i < medicalCount - 1; ++i) medicalQueue[i] = medicalQueue[i+1];
        medicalCount--;
        served = true;
    }
    else if (visaType == "BS" && businessCount > 0) {
        applicant = businessQueue[0];
        for (int i = 0; i < businessCount - 1; ++i) businessQueue[i] = businessQueue[i+1];
        businessCount--;
        served = true;
    }
    else if (visaType == "GO" && governmentCount > 0) {
        applicant = governmentQueue[0];
        for (int i = 0; i < governmentCount - 1; ++i) governmentQueue[i] = governmentQueue[i+1];
        governmentCount--;
        served = true;
    }

    if (served) {
        cout << "\n📢 [Counter " << counterNumber << "] Please serve: ";
        cout << "[" << applicant.visaType << "-" << applicant.tokenNumber << "]\n";

        int visaIndex = 0;
        if (visaType == "MED") visaIndex = 1;
        else if (visaType == "BS") visaIndex = 2;
        else if (visaType == "GO") visaIndex = 3;
        visaServed[visaIndex]++;

        counterServed[counterNumber-1][counterServedCount[counterNumber-1]++] = applicant;
    }

    return served;
}

// Finding longest queue
string getLongestQueue() {
    int maxSize = 0;
    string longestVisa = "";

    if (touristCount > maxSize) {
        maxSize = touristCount;
        longestVisa = "TR";
    }
    if (medicalCount > maxSize) {
        maxSize = medicalCount;
        longestVisa = "MED";
    }
    if (businessCount > maxSize) {
        maxSize = businessCount;
        longestVisa = "BS";
    }
    if (governmentCount > maxSize) {
        maxSize = governmentCount;
        longestVisa = "GO";
    }

    return longestVisa;
}

// next customer ke dak deya
void callNextCustomer() {
    cout << "Enter Counter Number (1-4): ";
    int counterNum;
    cin >> counterNum;
    if (counterNum < 1 || counterNum > 4) {
        cout << "\n🚫 Invalid counter number.\n";
        return;
    }

    string primaryVisa = getPrimaryVisa(counterNum);
    if (servePrimary(counterNum, primaryVisa)) return;

    
    string longestVisa = getLongestQueue();
    if (longestVisa != "") {
        servePrimary(counterNum, longestVisa);
    } else {
        cout << "\n⚡ Counter " << counterNum << " is idle. No applicants.\n";
    }
}

// Show daily summary
void dailySummary() {
    cout << "\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
    cout << "📝 DAILY SUMMARY REPORT\n";
    cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";

    cout << "\n📌 Applicants Served by Visa Type:\n";
    cout << "┌────────────────────────────┬───────────────┐\n";
    cout << "│ Visa Type                  │ Applicants    │\n";
    cout << "├────────────────────────────┼───────────────┤\n";

    cout << "│ " << setw(26) << left << getFullForm("TR") << "│ " << setw(13) << right << visaServed[0] << "│\n";
    cout << "│ " << setw(26) << left << getFullForm("MED") << "│ " << setw(13) << right << visaServed[1] << "│\n";
    cout << "│ " << setw(26) << left << getFullForm("BS") << "│ " << setw(13) << right << visaServed[2] << "│\n";
    cout << "│ " << setw(26) << left << getFullForm("GO") << "│ " << setw(13) << right << visaServed[3] << "│\n";

    cout << "└────────────────────────────┴───────────────┘\n";

    cout << "\n📌 Applicants Served by Counter:\n";
    cout << "┌────────────┬───────────────────────────────┐\n";
    cout << "│ Counter    │ Applicants Served             │\n";
    cout << "├────────────┼───────────────────────────────┤\n";
    for (int i = 0; i < COUNTERS; ++i) {
        cout << "│ " << setw(10) << left << ("Counter " + to_string(i+1)) << "│ ";
        for (int j = 0; j < counterServedCount[i]; ++j) {
            cout << counterServed[i][j].visaType << "-" << counterServed[i][j].tokenNumber << " ";
        }
        cout << "\n";
    }
    cout << "└────────────┴───────────────────────────────┘\n";

    cout << "\n📌 Idle Counters:\n";
    bool anyIdle = false;
    for (int i = 0; i < COUNTERS; ++i) {
        if (counterServedCount[i] == 0) {
            cout << "• Counter " << (i+1) << endl;
            anyIdle = true;
        }
    }
    if (!anyIdle) cout << "• None\n";

    cout << "\n✅ Total Applicants Served: " << totalApplicants << endl;
    cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
}

// Show Menu
void showMenu() {
    cout << "\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
    cout << "🚪 Embassy Consulate Line ..Visa with Rupam.co 🚪\n";
    cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
    cout << "1️⃣  Request Token\n";
    cout << "2️⃣  Call Next Customer at Counter\n";
    cout << "3️⃣  Show Daily Summary Report\n";
    cout << "4️⃣  Exit\n";
    cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n";
    cout << "Enter your choice: ";
}

// Main function
int main() {
    int choice;
    while (true) {
        showMenu();
        cin >> choice;
        if (choice == 1) {
            requestToken();
        }
        else if (choice == 2) {
            callNextCustomer();
        }
        else if (choice == 3) {
            dailySummary();
        }
        else if (choice == 4) {
            cout << "\n👋 Exiting the System. Thank you! (R.one)\n";
            break;
        }
        else {
            cout << "\n🚫 Invalid Choice. Please try again.\n";
        }
    }
    return 0;
}
