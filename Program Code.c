#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char med[50][15], dis[50][20]; int price[50];

int num = 0;

char name[20];
int age;

struct node{
	char medicine[15];
	char disease[20];
	int price;
	struct node *left;
	struct node *right;
};

struct node *createNewNode1(char medicine[], char disease[], int price){
	struct node *newNode = (struct node *) malloc(sizeof(struct node));
	
	strcpy(newNode->medicine, medicine);
	strcpy(newNode->disease, disease);
	newNode->price = price;
	newNode->left = NULL;
	newNode->right = NULL;
	return newNode;
}

struct trolley{
	char medicine[15];
	char disease[20];
	int price;
	int num;
	struct trolley *next;
};

struct trolley *head = NULL;

struct trolley *createNewNode2(char medicine[], char disease[], int price, int num){
	struct trolley *newNode = (struct trolley *) malloc(sizeof(struct trolley));
	
	strcpy(newNode->medicine, medicine);
	strcpy(newNode->disease, disease);
	newNode->price = price;
	newNode->num = num;
	newNode->next = NULL;
	
	return newNode;
}

void insertTrolley(char medicine[], char disease[], int price, int num) // Function memasukan obat yang dipilih customer ke dalam keranjang
{
	struct trolley *newNode = createNewNode2(medicine, disease, price, num);
	
	if (head == NULL){
		head = newNode;
	} else {
		newNode->next = head;
		head = newNode;
	}
}

struct node *insert(char medicine[], char disease[], int price, struct node *root){ // Function untuk memasukan daftar obat ke dalam program
	
	if(root == NULL){
		return createNewNode1(medicine, disease, price);
	} else if ((strcmp(medicine, root->medicine)) < 0){
		root->left = insert(medicine, disease, price, root->left);
	} else if ((strcmp(medicine, root->medicine)) > 0){
		root->right = insert(medicine, disease, price, root->right);
	}
	
	return root;
}

int inOrder(struct node *root){ 
	
	if(root != NULL){
		inOrder(root->left);
		printf("- %s (%s)\t-> Rp.%d,-\n", root->medicine, root->disease, root->price);
		inOrder(root->right);
	}
}

struct node* search(char medicine[], struct node* root) 
{
    if (root == NULL || (strcmp(medicine, root->medicine)) == 0){
		return root;
	} else if ((strcmp(medicine, root->medicine)) < 0){
		return search(medicine, root->left);
	} else if ((strcmp(medicine, root->medicine)) > 0){
		return search(medicine, root->right);
	}
}

void clear() // Function tempat keranjang customer
{
	struct trolley *curr = head;
	while (curr != NULL){
		struct trolley *temp = curr;
		curr = curr->next;
		free(temp);
	}
}

void payment(){ // Function dimana customer membayar total belanjanya
	
	int total = 0;
	struct trolley *curr = head;
	
	// Menampilkan isi keranjang customer
	while (curr != NULL){
		printf("%s (Rp.%d,-/satuan)\n", curr->medicine, curr->price);
		printf("- Quantity: %d\n", curr->num);
		printf("- Total: %d\n\n", curr->price * curr->num);
		total += (curr->price * curr->num);
		curr = curr->next;
	}
	
	// Menunjukkan total harga yang harus dibayar customer
	printf("Total Price: %d\n", total);
	
	// Menaruh data customer ke dalam file
	FILE *fa;
	fa = fopen("Customer List.txt", "a");
	fprintf(fa, "%s#%d#%d\n", name, age, total);
	fclose(fa);
	
	// Menghapus obat dalam keranjang
	clear();
}

void buy(struct node *root){ // Function dimana customer dapat memilih obat untuk dibeli
	char medicine[15], confirm; int input, item;
	
	do{
		// Menampilkan daftar obat yang sudah disorting
		printf("----------List of Medicine----------\n");
		inOrder(root);
		printf("\n1. Buy Medicine\n");
		printf("0. Exit\n");
		printf("Masukan Pilihan:\n");
		printf(">> ");
		scanf("%d", &input);
		if (input == 1){
			do {
				// Customer diminta untuk memasukan obat yang ingin dibeli
				printf("-----------------------------------\n");
				printf("Input Medicine: ");
				scanf(" %[^\n]", medicine);
				
				// Mencari obat yang dicari customer
				struct node *temp = search(medicine, root);
				if (temp == NULL){
					printf("\nThe Medicine Can't Be Found\n");
					
					printf("Do You Want To Try Again? (Y/N)");
					printf(">> ");
					scanf(" %c", &confirm);
					if (confirm == 'Y' || confirm == 'y'){
						system("cls");
						printf("----------List of Medicine----------\n");
						inOrder(root);
					} else if (confirm == 'N' || confirm == 'n'){
						;
					}
				} else {
					printf("Medicine: %s\n", temp->medicine);
					// Customer diminta untuk memasukan berapa jumlah yang ingin dibeli
					printf("Quantity: ");
					scanf("%d", &item);
					insertTrolley(temp->medicine, temp->disease, temp->price, item);
					
					// Customer akan ditanya apa ingin membeli obat lain
					printf("Would You Like To Buy Another? (Y/N)");
					printf(">> ");
					scanf(" %c", &confirm);
					system("cls");
					if (confirm == 'Y' || confirm == 'y'){
						printf("----------List of Medicine----------\n");
						inOrder(root);
					} else if (confirm == 'N' || confirm == 'n'){
						payment();
					}
				}
			} while (confirm == 'Y' || confirm == 'y');
		} else if (input == 0){
			clear();
		} else {
			printf("Wrong Input. Please Try Again\n");
			system("pause");
			system("cls");
		}
	} while (input != 0 && input != 1);
}

void customer(struct node *root){ // Tampilan untuk customer
	// Customer diminta untuk memasukan data diri
	printf("\nName: ");
	scanf(" %[^\n]", name);
	printf("Age: ");
	scanf("%d", &age);
	system("pause");
	system("cls");
	
	buy(root);
}

void insertMed(struct node *root){
	FILE *fa;
	char medName[15], medType[20]; int medPrice;
	
	printf("----------Insert Medicine----------\n");
	printf("Medicine's Name: ");
	scanf(" %[^\n]", medName);
	printf("Medicine's Type: ");
	scanf(" %[^\n]", medType);
	printf("Medicine's Price: ");
	scanf("%d", &medPrice);
	
	root = insert(medName, medType, medPrice, root);
	
	strcpy(med[num], medName);
	strcpy(dis[num], medType);
	price[num] = medPrice;
	
	fa = fopen("Medicine.txt", "a");
	fprintf(fa, "%s#%s#%d\n", medName, medType, medPrice);
	fclose(fa);
	
	printf("\nThe Medicine Has Been Input\n");
	num++;
}

void updateMed(struct node *root){
	
	FILE *fw, *fa; char medicine[15], confirm;
	do {
		printf("-----------------------------------\n");
		printf("Input Medicine You Want To Update!!\n");
		printf("Input Medicine: ");
		scanf(" %[^\n]", medicine);
		
		struct node *temp = search(medicine, root);
		if (temp == NULL){
			printf("\nThe Medicine Can't Be Found\n");
			
			printf("Do You Want To Try Again? (Y/N)");
			printf(">> ");
			scanf(" %c", &confirm);
			system("cls");
			if (confirm == 'Y' || confirm == 'y'){
				;
			} else if (confirm == 'N' || confirm == 'n'){
				;
			}
		} else {
			int input, i;
			
			system("cls");
			do {
				printf("----------Update Medicine----------\n");
				printf("1. Medicine's Name\n");
				printf("2. Medicine's Type\n");
				printf("3. Medicine's Price\n");
				printf("0. Exit\n");
				printf("Masukan Pilihan:\n");
				printf(">> ");
				scanf("%d", &input);
				switch (input){
					case 1:
						printf("------------------------------\n");
						printf("\nMedicine's Name: ");
						
						char medName[15];
						scanf(" %[^\n]", medName);
						
						strcpy(temp->medicine, medName);
						
						for (i = 0; i < num; i++){
							if ((strcmp(med[i], medicine)) == 0){
								strcpy(med[i], medName);
								break;
							}
						}
						
						fw = fopen("Medicine.txt", "w");
						fclose(fw);
						
						fa = fopen("Medicine.txt", "a");
						
						for (i = 0; i < num; i++){
							fprintf(fa, "%s#%s#%d\n", med[i], dis[i], price[i]);
						}
						fclose(fa);
						printf("\nThe Medicine Has Been Updated\n");
						system("pause");
						system("cls");
						break;
					case 2:
						printf("------------------------------\n");
						printf("\nMedicine's Type: ");
						
						char medType[20];
						scanf(" %[^\n]", medType);
						
						strcpy(temp->disease, medType);
						
						for (i = 0; i < num; i++){
							if ((strcmp(med[i], medicine)) == 0){
								strcpy(dis[i], medType);
								break;
							}
						}
						
						fw = fopen("Medicine.txt", "w");
						fclose(fw);
						
						fa = fopen("Medicine.txt", "a");
						
						for (i = 0; i < num; i++){
							fprintf(fa, "%s#%s#%d\n", med[i], dis[i], price[i]);
						}
						fclose(fa);
						printf("\nThe Medicine Has Been Updated\n");
						system("pause");
						system("cls");
						break;
					case 3:
						printf("------------------------------\n");
						printf("\nMedicine's Price: ");
						
						int medPrice;
						scanf("%d", &medPrice);
						
						temp->price = medPrice;
						
						for (i = 0; i < num; i++){
							if ((strcmp(med[i], medicine)) == 0){
								price[i] = medPrice;
								break;
							}
						}
						
						fw = fopen("Medicine.txt", "w");
						fclose(fw);
						
						fa = fopen("Medicine.txt", "a");
						
						for (i = 0; i < num; i++){
							fprintf(fa, "%s#%s#%d\n", med[i], dis[i], price[i]);
						}
						fclose(fa);
						printf("\nThe Medicine Has Been Updated\n");
						system("pause");
						system("cls");
						break;
					case 0:
						system("cls");
						break;
					default:
						printf("Wrong Input. Please Try Again\n");
						system("pause");
						system("cls");
						break;
				}
			} while (input != 0);
			system("pause");
		}
	} while (confirm == 'Y' || confirm == 'y');
}

struct node *findMin(struct node *root){
	while (root->left != NULL){
		root = root->left;
	}
	return root;
}

struct node *removeMed(char medicine[], struct node *root){
	
	if ((strcmp(medicine, root->medicine)) > 0){
		root->right = removeMed(medicine, root->right);
	} else if ((strcmp(medicine, root->medicine)) < 0){
		root->left = removeMed(medicine, root->left);
	} else {
		if (root->left == NULL && root->right == NULL){ // no child
			struct node *temp = root;
			root = NULL;
			free(temp);
			return root;
		} else if (root->right == NULL){ // 1 left child
			struct node *temp = root;
			root = root->left;
			free(temp);
		} else if (root->left == NULL){ // 1 right child
			struct node *temp = root;
			root = root->right;
			free(temp);
		} else { // 2 child
			struct node *minValue = findMin(root->right);
			strcpy(root->medicine, minValue->medicine);
			root->right = removeMed(minValue->medicine, root->right);
		}
	}
			
	return root;
}

void deleteMed(struct node *root){
	
	char medicine[15], confirm; int i;
	do {
		printf("-----------------------------------\n");
		printf("Input Medicine You Want To Delete!!\n");
		printf("Input Medicine: ");
		scanf(" %[^\n]", medicine);
		
		struct node *temp = search(medicine, root);
		if (temp == NULL){
			printf("\nThe Medicine Can't Be Found\n");
			
			printf("Do You Want To Try Again? (Y/N)");
			printf(">> ");
			scanf(" %c", &confirm);
			system("cls");
			if (confirm == 'Y' || confirm == 'y'){
				;
			} else if (confirm == 'N' || confirm == 'n'){
				;
			}
		} else {
			root = removeMed(medicine, root);
			
			int pos = 0;
			for (i = 0; i < num; i++){
				if ((strcmp(med[i], medicine)) != 0){
					pos++;
				} else {
					printf("%s %s %d\n", med[i], dis[i], price[i]);
					break;
				}
			}
			
			for (i = pos; i < num-1; i++){
				strcpy(med[i], med[i+1]);
				strcpy(dis[i], dis[i+1]);
				price[i] = price[i+1];
			}
			
			num--;
			
			for (i = 0; i < num; i++){
				printf("%s %s %d\n", med[i], dis[i], price[i]);
			}
			
			FILE *fw, *fa;
			fw = fopen("Medicine.txt", "w");
			fclose(fw);
			
			fa = fopen("Medicine.txt", "a");
			for (i = 0; i < num; i++){
				fprintf(fa, "%s#%s#%d\n", med[i], dis[i], price[i]);
			}
			fclose(fa);
			
			printf("\nThe Medicine Has Been Deleted\n");
			system("pause");
		}
	} while (confirm == 'Y' || confirm == 'y');
}

void edit(struct node *root){ // Admin dapat mengedit daftar obat
	int input;
	
	do {
		printf("----------Edit List of Medicine----------\n");
		printf("1. Insert Medicine\n");
		printf("2. Update Medicine\n");
		printf("3. Delete Medicine\n");
		printf("0. Exit\n");
		printf("Masukan Pilihan:\n");
		printf(">> ");
		scanf("%d", &input);
		switch (input){
			case 1:
				printf("\n");
				insertMed(root);
				system("pause");
				system("cls");
				break;
			case 2:
				printf("\n----------List of Medicine----------\n");
				inOrder(root);
				printf("\n");
				updateMed(root);
				system("cls");
				break;
			case 3:
				printf("\n----------List of Medicine----------\n");
				inOrder(root);
				printf("\n");
				deleteMed(root);
				
				system("cls");
				break;
			case 0:
				break;
			default:
				printf("Wrong Input. Please Try Again\n");
				system("pause");
				system("cls");
				break;
		}
	} while (input != 0);
}

void admin(struct node *root){ // Tampilan untuk admin
	FILE *fr; int input;
	
	// Menampilkan menu admin
	do {
		printf("----------Welcome Admin----------\n");
		printf("1. Edit List Of Medicine\n");
		printf("2. List Of Customer\n");
		printf("0. Exit\n");
		printf("Masukan Pilihan:\n");
		printf(">> ");
		scanf("%d", &input);
		switch (input){
			case 1:
				system("cls");
				edit(root);
				system("cls");
				break;
			case 2:
				// Mengambil data customer dari file
				printf("----------Customer List----------\n");
				
				if ((fr = fopen("Customer List.txt", "r")) != NULL){
					fr = fopen("Customer List.txt", "r");
					
					char custName[20]; int custAge, total, i = 0;
					
					printf("\n");
					while (fscanf(fr, "%[^#]#%d#%d\n", custName, &custAge, &total) != EOF){
						printf("%d. %s(%d thn) - %d\n", i+1, custName, custAge, total);
						i++;
					}
					fclose(fr);
					
					system("pause");
					system("cls");
				}
				break;
			case 0:
				break;
			default:
				printf("Wrong Input. Please Try Again\n");
				system("pause");
				system("cls");
				break;
		}
	} while (input != 0);
}

int main()
{FILE *fr, *fw; int input; char user[10], password[15];
	
	struct node *root = NULL;
	
	// Buka file untuk mengambil daftar obat ke dalam program
	if ((fr = fopen("Medicine.txt", "r")) != NULL){
		fr = fopen("Medicine.txt", "r");
		
		// Data yang diambil akan disorting menggunakan BST
		while (fscanf(fr, "%[^#]#%[^#]#%d\n", med[num], dis[num], &price[num]) != EOF){
			root = insert(med[num], dis[num], price[num], root);
			num++;
		}
		fclose(fr);
	} else {
		fw = fopen("Medicine.txt", "w");
		fclose(fw);
	}
	
	// Menampilkan menu
	do {
		printf("|----------Welcome----------|\n");
		printf("1. Customer\n");
		printf("2. Admin\n");
		printf("0. Exit\n");
		printf("Masukan Pilihan:\n");
		printf(">> ");
		scanf("%d", &input);
		switch (input){
			case 1:
				printf("--------------------\n");
				customer(root);
				system("pause");
				system("cls");
				break;
			case 2:
				printf("--------------------\n");
				printf("Username: ");
				scanf(" %s", user);
				printf("Password: ");
				scanf(" %s", password);
				if ((strcmp(user, "admin")) == 0 && (strcmp(password, "1234")) == 0){
					system("cls");
					admin(root);
					system("cls");
				} else {
					printf("Wrong Input\n");
					system("pause");
					system("cls");
					break;
				}
				break;
			case 0:
				printf("Thank You For Coming!!!\n");
				break;
			default:
				printf("Wrong Input. Please Try Again\n");
				system("pause");
				system("cls");
				break;
		}
	} while (input != 0);
	
	return 0;
}
