#define MAX_PRODUCTS 50  // Allow up to 50 products from file
#define MAX_CART 10

struct Order{
    char name[30];
    int quantity;
    float unit_price;
    float total_price;
};

int loadProducts(struct Product products[]) {
    FILE *fp = fopen("products.txt", "r");
    if (!fp) {
        printf("Error: Could not open products.txt\n");
        return 0;
    }
    int count = 0;
    while (count < MAX_PRODUCTS &&
           fscanf(fp, "%d %s %f %d\n",
                  &products[count].proNumber,
                  products[count].name,
                  &products[count].price,
                  &products[count].quantity) == 4) {
        count++;
    }
    fclose(fp);
    return count;
}

void showProducts(struct Product products[], int size){
    system("cls");
    printf("\nAvailable Products:\n");
    printf("---------------------------------------------------\n");
    printf("ID\tProduct\t\tPrice\tQuantity\n");
    printf("---------------------------------------------------\n");
    int i = 0;
    for (i = 0; i < size; i++)
    {
        printf("%d\t%-10s\t%.2f\t%d\n", products[i].proNumber, products[i].name, products[i].price, products[i].quantity);
    }
    printf("---------------------------------------------------\n");
}

void saveOrder(struct Order cart[], int cart_size, float grand_total){
    //Get current order ID from file
    int order_id = 1;
    FILE *id_fp = fopen("order_id.txt", "r");
    if (id_fp != NULL){
        fscanf(id_fp, "%d", &order_id);
        fclose(id_fp);
    }
    FILE *fp = fopen("orders.txt", "a");
    if (fp != NULL){
        fprintf(fp, "----------- Order ID: %03d -----------\n", order_id);
        int i = 0;
        for (i = 0; i < cart_size; i++){
            fprintf(fp, "%-15s\tQty: %d\tUnit: %.2f\tTotal: %.2f\n",
                    cart[i].name, cart[i].quantity, cart[i].unit_price, cart[i].total_price);
        }
        fprintf(fp, "Grand Total: %.2f\n\n", grand_total);
        fclose(fp);
        printf("\nOrder saved successfully as Order ID: %03d.\n", order_id);
    }else{
        printf("\nError: Could not save order.\n");
        return;
    }
    // Update order_id.txt
    order_id++;
    id_fp = fopen("order_id.txt", "w");
    if (id_fp != NULL){
        fprintf(id_fp, "%d", order_id);
        fclose(id_fp);
    }
    // Also append to sales.txt
    FILE *fsales = fopen("sales.txt", "a");
    if (fsales != NULL){
    	int i = 0;
        fprintf(fsales, "----------- Order ID: %03d -----------\n", order_id - 1);
        for (i= 0; i < cart_size; i++){
            fprintf(fsales, "%-15s\tQty: %d\tUnit: %.2f\tTotal: %.2f\n",
                    cart[i].name, cart[i].quantity, cart[i].unit_price, cart[i].total_price);
        }
        fprintf(fsales, "Grand Total: %.2f\n\n", grand_total);
        fclose(fsales);
    }
}

void viewOrders(){
    system("cls");
    FILE *fp = fopen("orders.txt", "r");
    if (!fp){
        printf("No past orders found.\n");
        return;
    }
    char line[256];
    printf("\n---------- Order History ----------\n");
    while (fgets(line, sizeof(line), fp)){
        printf("%s", line);
    }
    fclose(fp);
    printf("\nPress Enter to return to menu...");
    getchar();
    getchar(); // Pause
}

void custModule(){
    struct Product products[MAX_PRODUCTS];
    int product_count = loadProducts(products);
    if (product_count == 0) {
        printf("No products available to display. Please contact admin.\n");
        printf("Press Enter to return to menu...");
        getchar();
        getchar();
        return;
    }

    struct Order cart[MAX_CART];
    int cart_size = 0;
    int choice;
    char exit_choice;
    do{
        system("cls");
        while (1){
          	system("cls");
			printf("\n\n   WELCOME TO CUSTOMER SECTION\n\n");
			printf("==========================================\n");
			printf("\n\t1:    Order Products\n");
			printf("\n\t2:    View Past Records\n");
			printf("\n\t3:    Return to main menu\n");
			printf("\n==========================================\n");
			printf("\nSelect what do you want to do? ");
			while (1) {
		        if (scanf("%d", &choice) != 1) {
		            // Clear the invalid input (like characters or non-integer)
		            while (getchar() != '\n');
		            printf("Invalid input. Please enter a number (1-3): ");
		        } else {
		            break;
		        }
	    	}
	    
		char confirm[3];  // Space for 'y' or 'n' and the newline
    	int validInput = 0;  // Flag to check input validity
        if (choice == 1){
            do{
                showProducts(products, product_count);
                int product_id, quantity;
                float grand_total = 0;
                cart_size = 0;

                while (1){
                    printf("\nEnter product ID to add: ");
                    scanf("%d", &product_id);
                    if (product_id == 0) break;

                    int valid = 0;
                    int i = 0;
                    for (i = 0; i < product_count; i++){
                        if (products[i].proNumber == product_id){
                            valid = 1;
                            printf("Enter quantity for %s (Available: %d): ", products[i].name, products[i].quantity);
                            scanf("%d", &quantity);

                            if (quantity <= 0){
                                printf("Invalid quantity. Please enter a positive number.\n");
                                break;
                            }

                            if (quantity > products[i].quantity){
                                printf("Not enough stock! Only %d available.\n", products[i].quantity);
                                break;
                            }

                            products[i].quantity -= quantity;
                            strcpy(cart[cart_size].name, products[i].name);
                            cart[cart_size].quantity = quantity;
                            cart[cart_size].unit_price = products[i].price;
                            cart[cart_size].total_price = products[i].price * quantity;
                            grand_total += cart[cart_size].total_price;
                            cart_size++;
                            printf("\nDo you want to select another product? (Y/N): ");
                            char more;
                            scanf(" %c", &more);
                            if (more != 'Y' && more != 'y')
                                goto SHOW_BILL;
                            break;
                        }
                    }
                    if (!valid){
                        printf("Invalid product ID. Try again.\n");
                    }
                }

            SHOW_BILL:
                system("cls");
                printf("\n----------- Bill -----------\n");
                printf("Product\t\tQty\tUnit Price\tTotal\n");
                printf("--------------------------------------------------------\n");
                int i = 0;
                for (i = 0; i < cart_size; i++){
                    printf("%-10s\t%d\t%.2f\t\t%.2f\n",
                           cart[i].name, cart[i].quantity, cart[i].unit_price, cart[i].total_price);
                }
                printf("---------------------------------------------------\n");
                printf("Grand Total:\t\t\t%.2f\n", grand_total);

                char confirm;
                printf("\nConfirm order? (Y/N): ");
                scanf(" %c", &confirm);
                if (confirm == 'Y' || confirm == 'y'){
                    saveOrder(cart, cart_size, grand_total);
                    printf("\nPress Enter to return...");
                    getchar();
                    getchar();
                    break;
                }else{
                	int i = 0, j = 0;
                    // Restore product quantities because order cancelled
                    for (i = 0; i < cart_size; i++){
                        for (j = 0; j < product_count; j++){
                            if (strcmp(cart[i].name, products[j].name) == 0){
                                products[j].quantity += cart[i].quantity;
                                break;
                            }
                        }
                    }
                    printf("\nOrder cancelled. Quantities restored.\n");
                    printf("\nPress Enter to return...");
                    getchar();
                    getchar();
                    break;
                }
            } while (1);
        } else if (choice == 2){
            viewOrders();
        } else if (choice == 3){
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
                printf("Staying in the Customer Menu.\n");
                validInput = 1;
				Sleep(2000);
                system("cls");  // Proceed with admin menu
                break;  // Stay in the menu
            } else {
                printf("Invalid input. Please enter 'y' or 'n'.\n");
            }
            break;
        } else {
            printf("\nInvalid choice! Try again.\n");
            Sleep(2000);
			break;
        }
    }
    } while (1);
}
