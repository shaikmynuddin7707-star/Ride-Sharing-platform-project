#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

#define MAX 20
#define INF 99999

// ---------------- STRUCTURES ----------------

// Driver Structure
typedef struct {
    int id;
    char name[50];
    int available;  // 1 = Available, 0 = Not Available
} Driver;

// Passenger Structure
typedef struct {
    int id;
    char name[50];
} Passenger;

// ---------------- GLOBAL VARIABLES ----------------
Driver drivers[MAX];
Passenger passengers[MAX];
int graph[MAX][MAX];   // Adjacency matrix for distances

int driverCount = 0;
int passengerCount = 0;

// ---------------- FUNCTION DECLARATIONS ----------------
void addDriver();
void addPassenger();
void addEdge();
void displayDrivers();
void displayPassengers();
void displayGraph();
void findBestMatches();
void deleteDriver();
void deletePassenger();
int findDriverIndex(int id);
int findPassengerIndex(int id);
void initializeGraph();

// ---------------- INITIALIZE GRAPH ----------------
void initializeGraph() {
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            graph[i][j] = INF;
        }
    }
}

// ---------------- ADD DRIVER ----------------
void addDriver() {
    if (driverCount >= MAX) {
        printf("Driver limit reached!\n");
        return;
    }

    printf("Enter Driver ID: ");
    scanf("%d", &drivers[driverCount].id);

    printf("Enter Driver Name: ");
    scanf("%s", drivers[driverCount].name);

    drivers[driverCount].available = 1;
    driverCount++;

    printf("Driver added successfully!\n");
}

// ---------------- ADD PASSENGER ----------------
void addPassenger() {
    if (passengerCount >= MAX) {
        printf("Passenger limit reached!\n");
        return;
    }

    printf("Enter Passenger ID: ");
    scanf("%d", &passengers[passengerCount].id);

    printf("Enter Passenger Name: ");
    scanf("%s", passengers[passengerCount].name);

    passengerCount++;

    printf("Passenger added successfully!\n");
}

// ---------------- FIND INDEX FUNCTIONS ----------------
int findDriverIndex(int id) {
    for (int i = 0; i < driverCount; i++) {
        if (drivers[i].id == id)
            return i;
    }
    return -1;
}

int findPassengerIndex(int id) {
    for (int i = 0; i < passengerCount; i++) {
        if (passengers[i].id == id)
            return i;
    }
    return -1;
}

// ---------------- ADD EDGE ----------------
void addEdge() {
    int dId, pId, distance;

    printf("Enter Driver ID: ");
    scanf("%d", &dId);

    printf("Enter Passenger ID: ");
    scanf("%d", &pId);

    int dIndex = findDriverIndex(dId);
    int pIndex = findPassengerIndex(pId);

    if (dIndex == -1 || pIndex == -1) {
        printf("Invalid Driver or Passenger ID!\n");
        return;
    }

    printf("Enter Distance between Driver and Passenger: ");
    scanf("%d", &distance);

    graph[dIndex][pIndex] = distance;
    printf("Edge added successfully!\n");
}

// ---------------- DISPLAY DRIVERS ----------------
void displayDrivers() {
    if (driverCount == 0) {
        printf("No drivers available.\n");
        return;
    }

    printf("\n----- DRIVERS -----\n");
    printf("ID\tName\tAvailability\n");
    for (int i = 0; i < driverCount; i++) {
        printf("%d\t%s\t%s\n",
               drivers[i].id,
               drivers[i].name,
               drivers[i].available ? "Available" : "Busy");
    }
}

// ---------------- DISPLAY PASSENGERS ----------------
void displayPassengers() {
    if (passengerCount == 0) {
        printf("No passengers available.\n");
        return;
    }

    printf("\n----- PASSENGERS -----\n");
    printf("ID\tName\n");
    for (int i = 0; i < passengerCount; i++) {
        printf("%d\t%s\n",
               passengers[i].id,
               passengers[i].name);
    }
}

// ---------------- DISPLAY GRAPH ----------------
void displayGraph() {
    printf("\nDistance Matrix (Drivers → Passengers):\n");

    if (driverCount == 0 || passengerCount == 0) {
        printf("Insufficient data.\n");
        return;
    }

    printf("\t");
    for (int j = 0; j < passengerCount; j++) {
        printf("P%d\t", passengers[j].id);
    }
    printf("\n");

    for (int i = 0; i < driverCount; i++) {
        printf("D%d\t", drivers[i].id);
        for (int j = 0; j < passengerCount; j++) {
            if (graph[i][j] == INF)
                printf("INF\t");
            else
                printf("%d\t", graph[i][j]);
        }
        printf("\n");
    }
}

// ---------------- FIND BEST MATCHES (GREEDY) ----------------
void findBestMatches() {
    int assignedPassengers[MAX] = {0};

    printf("\n----- OPTIMAL DRIVER-PASSENGER MATCHES -----\n");

    for (int i = 0; i < driverCount; i++) {
        int minDist = INF;
        int bestPassenger = -1;

        if (!drivers[i].available)
            continue;

        for (int j = 0; j < passengerCount; j++) {
            if (!assignedPassengers[j] &&
                graph[i][j] < minDist) {
                minDist = graph[i][j];
                bestPassenger = j;
            }
        }

        if (bestPassenger != -1) {
            printf("Driver %s (ID:%d) → Passenger %s (ID:%d) | Distance: %d\n",
                   drivers[i].name, drivers[i].id,
                   passengers[bestPassenger].name, passengers[bestPassenger].id,
                   minDist);

            assignedPassengers[bestPassenger] = 1;
            drivers[i].available = 0;
        }
    }
}

// ---------------- DELETE DRIVER ----------------
void deleteDriver() {
    int id, index;

    printf("Enter Driver ID to delete: ");
    scanf("%d", &id);

    index = findDriverIndex(id);
    if (index == -1) {
        printf("Driver not found!\n");
        return;
    }

    for (int i = index; i < driverCount - 1; i++) {
        drivers[i] = drivers[i + 1];
        for (int j = 0; j < passengerCount; j++) {
            graph[i][j] = graph[i + 1][j];
        }
    }

    driverCount--;
    printf("Driver deleted successfully!\n");
}

// ---------------- DELETE PASSENGER ----------------
void deletePassenger() {
    int id, index;

    printf("Enter Passenger ID to delete: ");
    scanf("%d", &id);

    index = findPassengerIndex(id);
    if (index == -1) {
        printf("Passenger not found!\n");
        return;
    }

    for (int i = index; i < passengerCount - 1; i++) {
        passengers[i] = passengers[i + 1];
        for (int j = 0; j < driverCount; j++) {
            graph[j][i] = graph[j][i + 1];
        }
    }

    passengerCount--;
    printf("Passenger deleted successfully!\n");
}

// ---------------- MAIN FUNCTION ----------------
int main() {
    int choice;

    initializeGraph();

    while (1) {
        printf("\n====== RIDE SHARING PLATFORM ======\n");
        printf("1. Add Driver\n");
        printf("2. Add Passenger\n");
        printf("3. Add Matching Edge\n");
        printf("4. Display Drivers\n");
        printf("5. Display Passengers\n");
        printf("6. Display Graph\n");
        printf("7. Find Best Matches\n");
        printf("8. Delete Driver\n");
        printf("9. Delete Passenger\n");
        printf("10. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addDriver(); break;
            case 2: addPassenger(); break;
            case 3: addEdge(); break;
            case 4: displayDrivers(); break;
            case 5: displayPassengers(); break;
            case 6: displayGraph(); break;
            case 7: findBestMatches(); break;
            case 8: deleteDriver(); break;
            case 9: deletePassenger(); break;
            case 10:
                printf("Exiting program...\n");
                exit(0);
            default:
                printf("Invalid choice! Try again.\n");
        }
    }

    return 0;
}