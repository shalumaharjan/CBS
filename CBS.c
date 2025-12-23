#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <windows.h>  // For Sleep, system("cls")
#include <conio.h>  // For getch()
#include "welcome.h"
#include "menu.h"
#include "adminModule.h"
#include "customerModule.h"

int main(){
	welcome();
	system("cls");
	char ch = '0';
	char dec; //for decision
	while(1){
		mainMenu();
	  	while(1){
	  		printf("\nSelect your choice: ");
            if (scanf(" %c", &ch) != 1 || (ch < '1' || ch > '3')) {
                while (getchar() != '\n'); // Clear any leftover input in the buffer
                printf("\nInvalid choice. Please enter a valid option (1, 2, or 3).\n");
                continue;  // Ask for input again
            }
            break;  // Exit the loop when a valid choice is entered
		}
	    switch(ch){
			case '1':
				system("cls"); //Windows-specific
				verifyPss();
				break;
			case '2':
				custModule();				
				break;
			case '3':
				system("cls");
				printf("\nAre you sure you want to exit?(Y/N) ");
				scanf(" %c", &dec);
				if (dec == 'Y' || dec == 'y'){
					printf("\n\nThank you for using the billing system.\n");
					exit(0);
				}else{
					printf("\nReturning to Main Menu....\n");
					Sleep(2000);
					system("cls");
					break; // If 'N' is pressed, go back to the main menu
				}	
			default:
				printf("Invalid choice. Try again.\n");
                printf("Press Enter to continue...");
                Sleep(2000);
                system("cls");
                return;
		}
		ch = '0'; // Reset for next loop	
	}
	return 0;
}

