#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_CUSTOMERS 100
#define MAX_PRODUCTS 50
#define MAX_CART_ITEMS 20
#define ADMIN_USERNAME "archit"
#define ADMIN_PASSWORD "mittal101"

struct Customer {
    char name[50];
    char address[100];
    char contact[15];
};

struct Product {
    char name[50];
    float price;
    float tax;
    float discount;
    int stock;
};

struct CartItem {
    int productIndex;
    int quantity;
    float total;
};

struct Customer customers[MAX_CUSTOMERS];
struct Product products[MAX_PRODUCTS];
struct CartItem cart[MAX_CART_ITEMS];

int customerCount = 0, productCount = 0, cartItemCount = 0;

void toLowerCase(char str[]) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower((unsigned char) str[i]);
    }
}

// Function Prototypes
void adminMenu();
void customerMenu();
void loadCustomersFromFile();
void saveCustomersToFile();
void loadProductsFromFile();
void saveProductsToFile();
void addCustomer();
void editCustomer();
void deleteCustomer();
void listCustomers();
void addProduct();
void listProducts();
void addToCart();
void viewCart();
void checkout(char customerName[]);
float calculateTotal(float price, float tax, float discount, int qty);
int adminLogin();

int main() {
    loadCustomersFromFile();
    loadProductsFromFile();

    while (1) {
        int role;
        printf("\n=== welcome to supermarket billing system ===\n");
        printf("1. administrator\n");
        printf("2. customer\n");
        printf("3. exit\n");
        printf("select your role: ");
        scanf("%d", &role);
        getchar(); // clear buffer

        switch (role) {
            case 1:
                if (adminLogin()) {
                    adminMenu();
                }
                break;
            case 2:
                customerMenu();
                break;
            case 3:
                saveCustomersToFile();
                saveProductsToFile();
                printf("exiting... thank you!\n");
                exit(0);
            default:
                printf("invalid choice!\n");
        }
    }
    return 0;
}

int adminLogin() {
    char username[50], password[50];

    printf("\n=== administrator login ===\n");
    printf("username: ");
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] = '\0';

    printf("password: ");
    fgets(password, sizeof(password), stdin);
    password[strcspn(password, "\n")] = '\0';

    if (strcmp(username, ADMIN_USERNAME) == 0 && strcmp(password, ADMIN_PASSWORD) == 0) {
        printf("login successful.\n");
        return 1;
    } else {
        printf("invalid credentials. access denied.\n");
        return 0;
    }
}

void adminMenu() {
    int choice;
    while (1) {
        printf("\n--- administrator menu ---\n");
        printf("1. add customer\n");
        printf("2. edit customer\n");
        printf("3. delete customer\n");
        printf("4. list customers\n");
        printf("5. add product\n");
        printf("6. list products\n");
        printf("7. back to main menu\n");
        printf("enter your choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1: addCustomer(); break;
            case 2: editCustomer(); break;
            case 3: deleteCustomer(); break;
            case 4: listCustomers(); break;
            case 5: addProduct(); break;
            case 6: listProducts(); break;
            case 7: return;
            default: printf("invalid choice!\n");
        }
    }
}

void customerMenu() {
    cartItemCount = 0; // reset cart
    char name[50];
    printf("\nenter your name: ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = '\0';
    toLowerCase(name);

    int choice;
    while (1) {
        printf("\n--- customer menu ---\n");
        printf("1. add product to cart\n");
        printf("2. view cart\n");
        printf("3. checkout\n");
        printf("4. back to main menu\n");
        printf("enter your choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1: addToCart(); break;
            case 2: viewCart(); break;
            case 3: checkout(name); return;
            case 4: return;
            default: printf("invalid choice!\n");
        }
    }
}

void loadCustomersFromFile() {
    FILE *fp = fopen("customers.txt", "r");
    if (!fp) return;
    while (fscanf(fp, "%49[^,],%99[^,],%14[^\n]\n", customers[customerCount].name, customers[customerCount].address, customers[customerCount].contact) == 3) {
        toLowerCase(customers[customerCount].name);
        customerCount++;
    }
    fclose(fp);
}

void saveCustomersToFile() {
    FILE *fp = fopen("customers.txt", "w");
    for (int i = 0; i < customerCount; i++) {
        fprintf(fp, "%s,%s,%s\n", customers[i].name, customers[i].address, customers[i].contact);
    }
    fclose(fp);
}

void loadProductsFromFile() {
    FILE *fp = fopen("products.txt", "r");
    if (!fp) return;
    while (fscanf(fp, "%49[^,],%f,%f,%f,%d\n", products[productCount].name, &products[productCount].price,
                  &products[productCount].tax, &products[productCount].discount, &products[productCount].stock) == 5) {
        toLowerCase(products[productCount].name);
        productCount++;
    }
    fclose(fp);
}

void saveProductsToFile() {
    FILE *fp = fopen("products.txt", "w");
    for (int i = 0; i < productCount; i++) {
        fprintf(fp, "%s,%.2f,%.2f,%.2f,%d\n", products[i].name, products[i].price, products[i].tax,
                products[i].discount, products[i].stock);
    }
    fclose(fp);
}

void addCustomer() {
    struct Customer c;
    printf("enter name: "); fgets(c.name, sizeof(c.name), stdin); c.name[strcspn(c.name, "\n")] = 0;
    toLowerCase(c.name);
    printf("enter address: "); fgets(c.address, sizeof(c.address), stdin); c.address[strcspn(c.address, "\n")] = 0;
    printf("enter contact: "); fgets(c.contact, sizeof(c.contact), stdin); c.contact[strcspn(c.contact, "\n")] = 0;
    customers[customerCount++] = c;
    saveCustomersToFile();
    printf("customer added.\n");
}

void editCustomer() {
    char name[50];
    printf("enter customer name to edit: ");
    fgets(name, sizeof(name), stdin); name[strcspn(name, "\n")] = 0;
    toLowerCase(name);

    for (int i = 0; i < customerCount; i++) {
        if (strcmp(customers[i].name, name) == 0) {
            printf("enter new name: "); fgets(customers[i].name, 50, stdin); customers[i].name[strcspn(customers[i].name, "\n")] = 0;
            toLowerCase(customers[i].name);
            printf("enter new address: "); fgets(customers[i].address, 100, stdin); customers[i].address[strcspn(customers[i].address, "\n")] = 0;
            printf("enter new contact: "); fgets(customers[i].contact, 15, stdin); customers[i].contact[strcspn(customers[i].contact, "\n")] = 0;
            saveCustomersToFile();
            printf("customer updated.\n");
            return;
        }
    }
    printf("customer not found.\n");
}

void deleteCustomer() {
    char name[50];
    printf("enter customer name to delete: ");
    fgets(name, sizeof(name), stdin); name[strcspn(name, "\n")] = 0;
    toLowerCase(name);

    for (int i = 0; i < customerCount; i++) {
        if (strcmp(customers[i].name, name) == 0) {
            for (int j = i; j < customerCount - 1; j++) {
                customers[j] = customers[j + 1];
            }
            customerCount--;
            saveCustomersToFile();
            printf("customer deleted.\n");
            return;
        }
    }
    printf("customer not found.\n");
}

void listCustomers() {
    if (customerCount == 0) { printf("no customers.\n"); return; }
    for (int i = 0; i < customerCount; i++) {
        printf("name: %s, address: %s, contact: %s\n", customers[i].name, customers[i].address, customers[i].contact);
    }
}

void addProduct() {
    struct Product p;
    printf("enter product name: "); fgets(p.name, sizeof(p.name), stdin); p.name[strcspn(p.name, "\n")] = 0;
    toLowerCase(p.name);
    printf("enter price: "); scanf("%f", &p.price);
    printf("enter tax (%%): "); scanf("%f", &p.tax);
    printf("enter discount (%%): "); scanf("%f", &p.discount);
    printf("enter stock: "); scanf("%d", &p.stock); getchar();
    products[productCount++] = p;
    saveProductsToFile();
    printf("product added.\n");
}

void listProducts() {
    if (productCount == 0) { printf("no products available.\n"); return; }
    for (int i = 0; i < productCount; i++) {
        printf("name: %s, price: %.2f, tax: %.2f%%, discount: %.2f%%, stock: %d\n",
               products[i].name, products[i].price, products[i].tax, products[i].discount, products[i].stock);
    }
}

void addToCart() {
    char pname[50];
    int qty, found = 0;

    printf("enter product name to add: ");
    fgets(pname, sizeof(pname), stdin); pname[strcspn(pname, "\n")] = 0;
    toLowerCase(pname);

    for (int i = 0; i < productCount; i++) {
        if (strcmp(products[i].name, pname) == 0) {
            found = 1;
            printf("enter quantity: ");
            scanf("%d", &qty); getchar();
            if (qty > products[i].stock) {
                printf("insufficient stock. available: %d\n", products[i].stock);
                return;
            }

            float total = calculateTotal(products[i].price, products[i].tax, products[i].discount, qty);
            products[i].stock -= qty;
            cart[cartItemCount++] = (struct CartItem){i, qty, total};
            saveProductsToFile();
            printf("item added to cart.\n");
            return;
        }
    }
    if (!found) printf("product not found.\n");
}

void viewCart() {
    if (cartItemCount == 0) { printf("cart is empty.\n"); return; }
    printf("\nyour cart:\n");
    for (int i = 0; i < cartItemCount; i++) {
        printf("product: %s, quantity: %d, total: %.2f\n", products[cart[i].productIndex].name,
               cart[i].quantity, cart[i].total);
    }
}

void checkout(char customerName[]) {
    if (cartItemCount == 0) { printf("your cart is empty.\n"); return; }

    float grandTotal = 0;
    printf("\n===== bill =====\n");
    printf("customer: %s\n", customerName);
    for (int i = 0; i < cartItemCount; i++) {
        printf("product: %s, qty: %d, total: %.2f\n", products[cart[i].productIndex].name,
               cart[i].quantity, cart[i].total);
        grandTotal += cart[i].total;
    }
    printf("grand total: %.2f\n", grandTotal);
    printf("===== thanks for shopping! visit again. =====\n");
}

float calculateTotal(float price, float tax, float discount, int qty) {
    float subtotal = price * qty;
    float taxed = subtotal + (subtotal * tax / 100);
    float final = taxed - (taxed * discount / 100);
    return final;
}
