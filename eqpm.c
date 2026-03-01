#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Change this:
#define FILE_PATH    "/home/imtiaz/Desktop/EQPM/eq.txt"


typedef struct
{
	char name[50];
    int bass;
    int mids;
    int treble;

} EQProfile;

EQProfile profiles[5];
int total = 0;

const char correct_user[] = "admin\n";
const char correct_pass[] = "admin\n";


void pressEnter();
void login();
void showMainMenu();
void createProfile();
void editProfile();
void viewProfiles();
void searchProfile();
void deleteProfile();
void drawGraph(EQProfile p);
int findProfile(char name[]);
void loadFromFile();
void saveToFile();
int getNumber(int min, int max, char msg[]);


int main()
{
    login();
    loadFromFile();

    while(1)
    {
        showMainMenu();
    }

    return 0;
}


void pressEnter()
{
    printf("\nPress Enter to continue...");
    getchar();
}


void login()
{
    char user[30], pass[30];
    int tries = 3;

    system("cls || clear");
    printf("\n[[ EQ PROFILE MANAGER ]]\n\n");

    while(tries--)
    {
        printf("Username: ");
        fgets(user, 30, stdin);

        printf("Password: ");
        fgets(pass, 30, stdin);

        if(strcmp(user, correct_user)==0 && strcmp(pass, correct_pass)==0)
        {
            printf("\nLogin Successful! Welcome, Admin.\n");
            pressEnter();
            return;
        }
        else
        {
            printf("\nWrong username or password. %d tries left.\n", tries);
            pressEnter();
            system("cls || clear");
            printf("\n[[ EQ PROFILE MANAGER ]]\n\n");
        }
    }

    printf("Too many wrong attempts.\n");
    exit(0);
}


void showMainMenu()
{
    system("cls || clear");
    printf("\n[[ Main Menu ]]\n\n");

    printf("[ 1. Create ]\n");
    printf("[ 2. View   ]\n");
    printf("[ 3. Search ]\n");
    printf("[ 4. Edit   ]\n");
    printf("[ 5. Delete ]\n");
    printf("[ 6. Exit   ]\n");

    int choice = getNumber(1, 6, "\nEnter your choice: ");

    switch(choice)
    {
        case 1:
            createProfile();
            break;
        case 2:
            viewProfiles();
            break;
        case 3:
            searchProfile();
            break;
        case 4:
            editProfile();
            break;
        case 5:
            deleteProfile();
            break;
        case 6:
            saveToFile();
            system("cls || clear");
            printf("\n[[ Exit ]]\n\n");
            printf("You have been logged out.\n");
            exit(0);
    }
    pressEnter();
}


int getNumber(int min, int max, char msg[])
{
    int n;

    while(1)
    {
        printf("%s", msg);

        if ((scanf("%d", &n) != 1) || n < min || n > max)
        {
            printf("Invalid! Enter '%d' to '%d'. \n", min, max);
            getchar();
        }
        else
        {
            getchar();
            return n;
        }
    }
}


void createProfile()
{
    system("cls || clear");
    printf("\n[[ Create New Profile ]]\n\n");

    if(total>=5)
    {
        printf("Maximum 5 profiles reached!\n");
        return;
    }

    EQProfile p;
    printf("Enter profile name: ");
    fgets(p.name, 50, stdin);
    p.name[strcspn(p.name, "\n")] = '\0';

    if(findProfile(p.name) != -1)
    {
        printf("This name already exists!\n");
        return;
    }

    p.bass   = getNumber(-10, 10, "Bass   (-10 to +10): ");
    p.mids   = getNumber(-10, 10, "Mids   (-10 to +10): ");
    p.treble = getNumber(-10, 10, "Treble (-10 to +10): ");

    profiles[total] = p;
    total++;

    saveToFile();

    printf("\nProfile '%s' created successfully!\n\n", p.name);
    drawGraph(p);
}


void editProfile()
{
    system("cls || clear");
    printf("\n[[ Edit Profile ]]\n\n");

    if(total==0)
    {
        printf("No profiles to edit.\n");
        return;
    }

    for(int i=0; i<total; i++)
        printf("[ %d ] %s\n", i+1, profiles[i].name);

    int index = getNumber(1, total, "\nChoose profile: ") - 1;
    EQProfile *p = &profiles[index];

    printf("\n\nCurrent settings:\n");
    drawGraph(*p);

    printf("\n");
    printf("[ 1. Bass   ]\n");
    printf("[ 2. Mids   ]\n");
    printf("[ 3. Treble ]\n");

    int band = getNumber(1, 3, "\nChoose band: ");
    int val = getNumber(-10, 10, "Enter new value: ");

    if(band==1)
		(*p).bass = val;
    else if(band==2)
		(*p).mids = val;
    else
		(*p).treble = val;

    saveToFile();
    printf("\nProfile updated!\n");
    drawGraph(*p);
}


void viewProfiles()
{
    system("cls || clear");
    printf("\n[[ View All Profiles ]]\n\n");

    if(total==0)
    {
        printf("No profiles saved yet.\n");
        return;
    }
	else
	{
		for(int i=0; i<total; i++)
		{
			printf("\n[ %d ] %s\n", i+1, profiles[i].name);
			drawGraph(profiles[i]);
		}
	}
}


void searchProfile()
{
    system("cls || clear");
    printf("\n[[ Search Profile ]]\n\n");

    if(total==0)
    {
        printf("No profiles available.\n");
        return;
    }

    char name[50];
    printf("Enter profile name: ");
    fgets(name, 50, stdin);
    name[strcspn(name, "\n")] = '\0';

    int i = findProfile(name);
    if(i==-1)
        printf("\nProfile '%s' not found!\n", name);
    else
    {
        printf("\nProfile found!\n\n");
        printf("[ %d ] %s\n", i+1, profiles[i].name);
        drawGraph(profiles[i]);
    }
}


void deleteProfile()
{
    system("cls || clear");
    printf("\n[[ Delete Profile ]]\n\n");

    if(total==0)
    {
        printf("No profiles to delete.\n");
        return;
    }

    for(int i=0; i<total; i++)
        printf("[ %d ] %s\n", i+1, profiles[i].name);

    int index = getNumber(1, total, "\nChoose profile to delete: ") - 1;

    printf("\nAre you sure you want to delete '%s'? (y/n): ", profiles[index].name);
    char confirm;
    scanf(" %c", &confirm);
    getchar();

    if(confirm != 'y' && confirm != 'Y')
    {
        printf("\nDelete cancelled.\n");
        return;
    }
    else
    {
		for(int i=index; i<total-1; i++)
		{
			profiles[i] = profiles[i+1];
		}
	}

    total--;
    saveToFile();

    printf("\nProfile deleted successfully!\n");
}


int findProfile(char name[])
{
    for(int i=0; i<total; i++)
    {
        if(strcmp(profiles[i].name, name) == 0)
			return i;
	}
    return -1;
}


void drawGraph(EQProfile p)
{
    printf("\n[[ EQ Profile: %s | Bass: %+d, Mids: %+d, Treble: %+d ]]\n\n",
           p.name, p.bass, p.mids, p.treble);

    int v[3] = {p.bass, p.mids, p.treble};
    char l[3][7] = {"BASS", "MIDS", "TREBLE"};

    for(int i=0; i<3; i++)
    {
        char s;
		if (v[i] >= 0)
			s = '+';
		else
			s = '-';

        int n;
		if (v[i] >= 0)
			n = v[i];
		else
			n = -v[i];

        printf("%-6s : ", l[i]);
        for(int j=0; j<n; j++)
            printf("%c", s);

        printf("\n");
    }
    printf("\n");
}


void loadFromFile()
{
    FILE *f = fopen(FILE_PATH, "r");
    if(f == NULL)
        return;

    while(total<5)
    {
        int success = fscanf(f, "%[^,],%d,%d,%d\n",
                             profiles[total].name,
                             &profiles[total].bass,
                             &profiles[total].mids,
                             &profiles[total].treble);

        if(success != 4)
            break;

        total++;
    }
    fclose(f);
}


void saveToFile()
{
    FILE *f = fopen(FILE_PATH, "w");
    if(f == NULL)
    {
        printf("Error: Could not save file!\n");
        return;
    }

    for(int i=0; i<total; i++)
    {
        fprintf(f, "%s,%d,%d,%d\n", profiles[i].name,
                profiles[i].bass, profiles[i].mids, profiles[i].treble);
    }
    fclose(f);
}
