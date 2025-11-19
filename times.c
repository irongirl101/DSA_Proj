#include <stdio.h>
#include <time.h>

int main() {
    time_t currentTime;
    struct tm *localTime;
    char dateBuffer[100];

    
    currentTime = time(NULL);

    localTime = localtime(&currentTime);

    
    strftime(dateBuffer, sizeof(dateBuffer), "%Y-%m-%d", localTime);


    printf("Current Date: %s\n", dateBuffer);

    return 0;
}