#define PRODUCT_FILE "products.txt"
#define SALES_FILE "sales.txt"

void addProduct();
void deleteProduct();
void updateProduct();
void viewSales();
void listAllProducts();

struct Product {
    int proNumber;
    char name[30];
    float price;
    int quantity;
};

void hidePassword(char *password) {
    int index = 0;
    char ch;
    while (1) {
        ch = getch();  // Read the character
        if (ch == '\r') {  // Enter key
            password[index] = '\0';
            break;
        } else if (ch == '\b') {  // Backspace
            if (index > 0) {
                index--;  
                printf("\b \b");  // Erase the asterisk
            }
        } else {
            password[index] = ch; 
            index++;
            printf("*"); 
        }
    }
    printf("\n");  // New line after password input
}

void storePassword(const char *password) {
    FILE *file = fopen("admin_password.dat", "wb");
    if (file != NULL) {
        fwrite(password, sizeof(char), strlen(password) + 1, file);
        fclose(file);
        printf("\nPassword stored successfully.\n");
    } else {
        printf("\nError storing password.\n");
    }
}

int checkPassword(const char *enteredPassword) {
    FILE *file = fopen("admin_password.dat", "rb");
    if (file == NULL) {
        printf("\nPassword not set yet. \nPlease set the password first.\n");
        return 0;
    }
    char storedPassword[30];
    fread(storedPassword, sizeof(char), sizeof(storedPassword), file);
    fclose(file);
    return strcmp(enteredPassword, storedPassword) == 0;
}

int verifyPss() {
    char password[30];
    int choice = 0;
    printf("\n\n          ADMIN SECTION\n\n");
	printf("===============================\n");

    // If no password exists, prompt user to set a new one
    FILE *file = fopen("admin_password.dat", "rb");
    if (file == NULL) {
        printf("\nNo password set. \nPlease set a new password: ");
        hidePassword(password);  // Hide password input
        storePassword(password);  // Store the new password
    } else {
        printf("\nEnter admin password: ");
        hidePassword(password); 
        if (checkPassword(password)) {
            printf("Access granted!\n");
            Sleep(2000);
            while (1){
            	system("cls");
				printf("\n\n   WELCOME TO ADMIN SECTION\n\n");
				printf("==========================================\n");
				printf("\n\t1:    Add New Product\n");
				printf("\n\t2:    Delete Product\n");
				printf("\n\t3:    Update Product Price\n");
				printf("\n\t4:    View all Sale Records\n");
				printf("\n\t5:    View all Products\n");
				printf("\n\t6:    Return to main menu\n");
				printf("\n==========================================\n");
				printf("\nSelect what do you want to do? ");
				while (1) {
			        if (scanf("%d", &choice) != 1) {
			            // Clear the invalid input (like characters or non-integer)
			            while (getchar() != '\n');
			            printf("Invalid input. Please enter a number (1-6): ");
			        } else {
			            break;
			        }
		    	}
		    char confirm[3];  // Space for 'y' or 'n' and the newline
    		int validInput = 0;  // Flag to check input validity
			switch(choice){
				case 1:
					addProduct();
			        break;
			    case 2:
			        deleteProduct();
			        break;
			    case 3:
			        updateProduct();
			        break;
			    case 4:
			        viewSales();
			        break;
			    case 5:
		            listAllProducts();
		            printf("\nPress Enter to return...");
				    getchar();
				    getchar();
		            break;
			    case 6:
			    	printf("Do you want to return to the main menu? (y/n): ");
	                while (getchar() != '\n'); // Consume leftover characters
	                fgets(confirm, sizeof(confirm), stdin);  // Read input as string
	
	                // Remove the newline character from confirm (if any)
	                confirm[strcspn(confirm, "\n")] = '\0';
	
	                if (confirm[0] == 'y' || confirm[0] == 'Y') {
	                    printf("Returning to main menu...\n");
	                    // Add delay to simulate transition
	                    validInput = 1;  // Exit admin menu
	                    Sleep(2000);
	                    system("cls");
	                    return;  // Exit to main menu
	                } else if (confirm[0] == 'n' || confirm[0] == 'N') {
	                    printf("Staying in the Admin Menu.\n");
	                    validInput = 1;
						Sleep(2000);
	                    system("cls");  // Proceed with admin menu
	                    break;  // Stay in the menu
	                } else {
	                    printf("Invalid input. Please enter 'y' or 'n'.\n");
	                }
	                break;
			    default:
			        printf("Invalid choice. Try again.\n");
			        Sleep(2000);
			        break;
			}
		}
		}else {
            printf("\nIncorrect password. Access denied.\n");
            Sleep(2000);
            system("cls");
            return;
        }
    }
    return 0;
}

void addProduct()
{
    FILE *fp = fopen(PRODUCT_FILE, "a+"); 
    if (!fp){
        printf("\nError opening product file.\n");
        return;
    }
    int n, lastProductNo = 0;
    struct Product p;

    // Find the last product number in the file
    while (fscanf(fp, "%d %s %f %d", &p.proNumber, p.name, &p.price, &p.quantity) == 4){
        if (p.proNumber > lastProductNo){
            lastProductNo = p.proNumber;
        }
    }
    printf("\nHow many products do you want to add? ");
    scanf("%d", &n);
    getchar();  // To consume the newline left by scanf
    // Array to store all product names for comparison
    char productNames[50][50]; // Assuming there are at most 100 products

    // Reset file pointer to start of the file to recheck existing names
    rewind(fp);
    int productCount = 0;
    // Load existing product names into the array for comparison
    while (fscanf(fp, "%d %s %f %d", &p.proNumber, p.name, &p.price, &p.quantity) == 4){
        strcpy(productNames[productCount], p.name);
        productCount++;
    }

    int i = 0, j = 0;
    for (i = 0; i < n; i++){
        p.proNumber = ++lastProductNo;  
        printf("\nAuto-generated Product Number: %d\n", p.proNumber);
        printf("Enter product name: ");
        scanf(" %[^\n]s", p.name);
        int foundDuplicate = 0;
        for (j = 0; j < productCount; j++){
            if (strcmp(p.name, productNames[j]) == 0){
                foundDuplicate = 1;
                break;  // No need to check further
            }
        }

        if (foundDuplicate){
            printf("\nProduct '%s' already exists in the list!\n", p.name);
            continue; 
        }

        printf("Enter price: ");
        scanf("%f", &p.price);
        printf("Enter quantity (number or kg.): ");
        scanf("%d", &p.quantity);

        fprintf(fp, "%d %s %.2f %d\n", p.proNumber, p.name, p.price, p.quantity);

        strcpy(productNames[productCount], p.name);
        productCount++;
        printf("\nProduct(s) added successfully!\n");
    }

    fclose(fp);
    printf("\nReturning to the Admin Menu.\n");
    Sleep(2000);
}
void deleteProduct(){
	system("cls");
    struct Product p;
    int deleteId, found = 0;

    // First open to display all products
    FILE *fp = fopen(PRODUCT_FILE, "r");
    if (!fp)
    {
        printf("\nError opening product file.\n");
        return;
    }

    // Display products
    printf("\n=============== List of All Products ===============\n");
    printf("Product No   Product Name         Price      Quantity(number or kg.)\n");
    printf("---------------------------------------------------------------\n");

    while (fscanf(fp, "%d %s %f %d", &p.proNumber, p.name, &p.price, &p.quantity) == 4)
    {
        printf("%-12d %-20s %-10.2f %d\n", p.proNumber, p.name, p.price, p.quantity);
    }
    printf("=========================================================\n");

    fclose(fp);  // Close after listing

    // Ask which product to delete
    printf("\nEnter the product number to delete: ");
    scanf("%d", &deleteId);
    getchar(); // Clear newline from buffer

    // Reopen file for deletion
    fp = fopen(PRODUCT_FILE, "r");
    FILE *temp = fopen("temp.txt", "w");

    if (!fp || !temp)
    {
        printf("\nError reopening files.\n");
        return;
    }

    while (fscanf(fp, "%d %s %f %d", &p.proNumber, p.name, &p.price, &p.quantity) == 4)
    {
        if (p.proNumber != deleteId)
        {
            fprintf(temp, "%d %s %.2f %d\n", p.proNumber, p.name, p.price, p.quantity);
        }
        else
        {
            found = 1;
            printf("\nDeleted: %d %s | Price: %.2f | Quantity: %d\n",
                   p.proNumber, p.name, p.price, p.quantity);
        }
    }

    fclose(fp);
    fclose(temp);

    remove(PRODUCT_FILE);
    rename("temp.txt", PRODUCT_FILE);

    if (found)
    {
        printf("\nProduct deleted successfully.\n");
        Sleep(2000);
    }
    else
    {
        printf("\nProduct with number %d not found.\n", deleteId);
        Sleep(2000);
    }
}


void updateProduct()
{
    FILE *fp = fopen(PRODUCT_FILE, "r");
    FILE *temp = fopen("temp.txt", "w");
    struct Product p;
    int updateId, found = 0;

    if (!fp || !temp){
        printf("\nError opening file.\n");
        return;
    }
    // Show products before updating
    listAllProducts(0);
    printf("\nEnter product number to update price: \n");
    scanf("%d", &updateId);
    while (fscanf(fp, "%d %s %f %d", &p.proNumber, p.name, &p.price, &p.quantity) == 4){
        if (p.proNumber == updateId){
            printf("Current price for '%s' is: %.2f\n", p.name, p.price);
            printf("Enter new price: ");
            scanf("%f", &p.price);
            found = 1;
        }
        fprintf(temp, "%d %s %.2f %d\n", p.proNumber, p.name, p.price, p.quantity);
    }
    fclose(fp);
    fclose(temp);
    remove(PRODUCT_FILE);
    rename("temp.txt", PRODUCT_FILE);
    if (found)
        printf("\nPrice updated successfully.\n");
    else
        printf("\nProduct not found.\n");
    Sleep(2000);
}

void viewSales() {
	system("cls");
    FILE *fp = fopen("sales.txt", "r");
    if (!fp) {
        printf("\nNo sales record found.\n");
        Sleep(2000);
        return;
    }
    printf("\nFile opened successfully.\n\n");
    printf("============ SALES RECORDS ============\n\n");
    Sleep(1000);
    char line[256];
    int newOrder = 1;
    while (fgets(line, sizeof(line), fp)) {
        // Remove trailing newline
        line[strcspn(line, "\n")] = 0;
        // Detect new order header (e.g., "---- Order ID: 001 ----")
        if (strncmp(line, "----------- Order ID", 13) == 0) {
            if (!newOrder) {
                printf("\n=============================================\n\n");
            }
            newOrder = 0;
            printf("\n%s\n", line);  // Print the order ID line  
        }
        else if (strncmp(line, "Grand Total", 11) == 0) {
            printf("\n---------------------------------------------\n");
            printf("%s\n", line);  
        }
        else if (line[0] != '\n' && line[0] != '-') {
            // Product line (assuming products have the format "Product Qty: x Unit: y Total: z")
            char product[30];
            int qty;
            float unit, total;

            // Try to parse the product line
            if (sscanf(line, "%49s Qty: %d Unit: %f Total: %f", product, &qty, &unit, &total) == 4) {
                printf("%-20s %-10d %-12.2f %-12.2f\n", product, qty, unit, total);   
            } else {
                continue; // If parsing fails, just continue
            }
        }
    }
    printf("\n=============================================\n");
    fclose(fp);
    
    printf("\nPress Enter to return...");
    getchar();
    getchar();
    return;
}


void listAllProducts(){
	system("cls");
    FILE *fp = fopen(PRODUCT_FILE, "r");
    struct Product p;

    if (!fp){
        printf("\nNo products found.\n");
        return;
    }

    printf("\n============== List of All Products ==============\n\n");
    printf("%-12s %-20s %-10s %-10s\n", "Product No", "Product Name", "Price", "Quantity(number or kg.)");
    printf("\n---------------------------------------------------------------\n");

    while (fscanf(fp, "%d %s %f %d", &p.proNumber, p.name, &p.price, &p.quantity) == 4)
    {
        printf("%-12d %-20s %-10.2f %-10d\n", p.proNumber, p.name, p.price, p.quantity);
    }

    fclose(fp);
    printf("\n=========================================================\n");
	
    
    return;
}
