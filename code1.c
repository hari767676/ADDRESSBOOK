#include<stdio.h>
#include<string.h>
#include<ctype.h>
#include<stdlib.h>

struct data
{
    char name[50];
    char mobile[12];
    char email[50];
    char address[100];
};

void add_contacts();
void copy(FILE *,FILE *);
void search_contact();
void print_contact();
void edit_contact();
void delete_contact();
int validate_mobile(char *,int);
int validate_email(char *);
int revalidation();

int main() 
{
    char ch='y';
    int choice;
    while (ch=='y' || ch=='Y')
    {
       system("clear");
       printf(" 1.Add\n 2.search\n 3.print\n 4.edit\n 5.delete\n Enter your choice: ");
       scanf("%d",&choice);
       switch(choice)
       {
        case 1:
            add_contacts();
            break;
        case 2:
            search_contact();
            break;
        case 3:
            print_contact();
            break;
        case 4:
            edit_contact();
            break;
        case 5:
            delete_contact();
            break;
       }
       printf("do you want to continue(y/n): ");
       scanf(" %c",&ch);
    }
}
//__________________________________________________________________________________________________________
void add_contacts()
{
    system("clear");
    struct data contacts;
    int count=0;
    FILE *fdata=fopen("data.csv","r");
    if(fdata== NULL)
    {
        printf("file not opened");
        return;
    }
    FILE *ftemp=fopen("temp.csv","w");
    fscanf(fdata,"%d\n",&count);
    fprintf(ftemp,"%d\n",count+1);
    for(int i=0;i<count;i++)
    {
        fscanf(fdata,"%[^,],%[^,],%[^,],%[^\n]\n",contacts.name,contacts.mobile,contacts.email,contacts.address);
        fprintf(ftemp,"%s,%s,%s,%s\n",contacts.name,contacts.mobile,contacts.email,contacts.address);
    }
    fclose(fdata);
    char buffer[50];
    label_0 : printf("Enter the name : ");
    scanf(" %[^\n]",buffer);
    for(int i=0;buffer[i]!='\0';i++)
    {
        if(isalpha(buffer[i]) == 0)
        {
            printf("!!!Error Enter Valid Name!!!\n");
            goto label_0;
        }
    }
    strcpy(contacts.name,buffer);

    label_1 : printf("Enter the mobile number : ");
    scanf(" %[^\n]",buffer);
    if(validate_mobile(buffer,0))        
        strcpy(contacts.mobile,buffer);
    else
    {
        if(revalidation())
            goto label_1;
        else
            return;
    }
    
    label_2 : printf("Enter the email : ");
    scanf(" %[^\n]",buffer);
    if (validate_email(buffer))
        strcpy(contacts.email,buffer);
    else
    {
        if(revalidation())
            goto label_2;
        else
            return;
    }
    printf("Enter the address : ");
    scanf(" %[^\n]",contacts.address);
    fprintf(ftemp,"%s,%s,%s,%s\n",contacts.name,contacts.mobile,contacts.email,contacts.address);
    fclose(ftemp);
    fdata=fopen("data.csv","w");
    ftemp=fopen("temp.csv","r");
    copy(fdata,ftemp);
    fclose(fdata);
    fclose(ftemp);
}

void copy(FILE *fdata,FILE *ftemp)
{
    char ch;
    while((ch=fgetc(ftemp))!=EOF)
    {
        fputc(ch,fdata);
    }
}

int validate_mobile(char mobile_number[], int val)
{
    for (int i = 0; mobile_number[i] != '\0'; i++)
    {
        if (isdigit(mobile_number[i]) == 0)
        {
            printf("Error enter valid number\n");
            return 0;
        }
    }
    if (strlen(mobile_number) != 10)
    {
        printf("Error mobile number should contain exactly 10 digits\n");
        return 0;
    }
    if (val == 0)
    {
        FILE *fdata = fopen("data.csv", "r");
        int count = 0;
        struct data contacts;
        fscanf(fdata, "%d\n", &count);
        for (int i = 0; i < count; i++)
        {
            fscanf(fdata, "%[^,],%[^,],%[^,],%[^\n]\n", contacts.name, contacts.mobile, contacts.email, contacts.address);
            if (strcmp(mobile_number, contacts.mobile) == 0)
            {
                printf("Same mobile number found.Enter Number again:\n");
                fclose(fdata);
                return 0;
            }
        }
        fclose(fdata);
    }
    return 1;
}

int validate_email(char email[]) 
{
    int at = -1, dot = -1;
    for (int i = 0; email[i] != '\0'; i++) 
    {
        if (email[i] == '@') 
        {
            if (at == -1) 
                at = i;
            else 
            {
                printf("Error: Invalid email!!!\n");
                return 0;
            }
        } 
        else if (email[i] == '.' && at != -1 && dot == -1) 
            dot = i;
    }

    if (at > 0 && dot > at + 1 && isalpha(email[at + 1]) && isalpha(email[dot + 1])) 
        return 1;
    
    printf("Error: Invalid email format\n");
    return 0;
}
//___________________________________________________________________________________________________________
void search_contact()
{
    system("clear");
    struct data contacts;
    FILE *fdata = fopen("data.csv", "r");
    if (fdata == NULL)
    {
        printf("No contacts found\n");
        return;
    }
    char search[30];
    printf("Enter the name to be searched: ");
    scanf(" %[^\n]", search);
    int count = 0, name_occ = 0;
    fscanf(fdata, "%d\n", &count);
    for (int i = 0; i < count; i++)
    {
        fscanf(fdata, "%[^,],%[^,],%[^,],%[^\n]\n", contacts.name, contacts.mobile, contacts.email, contacts.address);
        if ((strcmp(search, contacts.name)) == 0)
        {
            name_occ++;
        }
    }
    
    if (name_occ == 0)
    {
        printf("Contact Not Found\n");
        fclose(fdata);
        return;
    }
    rewind(fdata);
    fscanf(fdata, "%d\n", &count);
    char find_mobile[12];
    char input_buffer[12]; 
    if (name_occ > 1)
    {
        int valid_mobile = 0;
        do
        {
            printf("Multiple contacts found for same name. Enter mobile to confirm: ");
            scanf(" %[^\n]", input_buffer);
            if (validate_mobile(input_buffer, 1))
            {
                strcpy(find_mobile, input_buffer);
                valid_mobile = 1;
            }
            else
            {
                if (!revalidation())
                {
                    fclose(fdata);
                    return;
                }
                printf("Enter the Mobile Number: \n");
            }
        } while (!valid_mobile);
    }

    
    int found_flag = 0; 
    for (int i = 0; i < count; i++)
    {
        fscanf(fdata, "%[^,],%[^,],%[^,],%[^\n]\n", contacts.name, contacts.mobile, contacts.email, contacts.address);
        if (strcmp(search, contacts.name) == 0)
        {
            if (name_occ == 1 || strcmp(contacts.mobile, find_mobile) == 0)
            {
                found_flag = 1;
                break;
            }
        }
    }

    if (found_flag == 0)
    {
        printf("Contact Not Found\n");
        fclose(fdata);
        return;
    }

    printf("Contact Found\n");
    printf("______________________________\n");
    printf("Name: %s\nMobile: %s\nEmail: %s\nAddress: %s\n", contacts.name, contacts.mobile, contacts.email, contacts.address);
    printf("_______________________________\n");

    fclose(fdata);
}

//___________________________________________________________________________________________________________

void print_contact()
{
    system("clear");
    struct data contacts;
    FILE *fdata=fopen("data.csv","r");
    int count;
    fscanf(fdata,"%d",&count);
    if(count==0)
    {
        printf("No contacts found\n");
    }
    while(count!=0)
    {
        fscanf(fdata,"%[^,],%[^,],%[^,],%[^\n]\n",contacts.name,contacts.mobile,contacts.email,contacts.address);
        printf("%s\n%s\n%s\n%s\n",contacts.name,contacts.mobile,contacts.email,contacts.address);
        printf("_________________________");
        printf("\n");
        count=count-1;
    }
}
//_________________________________________________________________________________________________________
void edit_contact()
{
    system("clear");
    char search[30];
    struct data contacts;
    FILE *fdata = fopen("data.csv", "r");
    if (fdata == NULL)
    {
        printf("No contacts found\n");
        return;
    }
    FILE *ftemp = fopen("temp.csv", "w");
    int count;
    fscanf(fdata, "%d\n", &count);
    fprintf(ftemp, "%d\n", count);
    printf("Enter the name to edit: ");
    scanf(" %[^\n]",search);
    int name_occ=0;
    for(int i=0;i<count;i++)
    {
        fscanf(fdata,"%[^,],%[^,],%[^,],%[^\n]\n",contacts.name,contacts.mobile,contacts.email,contacts.address);
        if((strcmp(search,contacts.name))==0)
        {
            name_occ++;
        }
    }
    if(name_occ==0)
    {
        printf("Contact Not Found\n");
        return;
    }
    char find_mobile[11];
    char buffer[50];
    rewind(fdata);
    fscanf(fdata,"%d\n",&count);
    if(name_occ>1)
    {
        printf("Multiple contacts found for same name.enter mobile to confirm: ");
        label_3 : scanf(" %[^\n]",buffer);
        if(validate_mobile(buffer,1))
            strcpy(find_mobile,buffer);
        else
        {
            if(revalidation())
            {
                printf("Enter the Mobile Number : \n");
                goto label_3;
            }
            else
                return;
        }  
    }
    int flag=0;
    for(int i=0;i<count;i++)
    {
        fscanf(fdata,"%[^,],%[^,],%[^,],%[^\n]\n",contacts.name,contacts.mobile,contacts.email,contacts.address);
        if((strcmp(search,contacts.name))==0)
        {
            if(name_occ==1 || (strcmp(contacts.mobile,find_mobile)==0))
            {
                flag=1;
                printf("Contact found, Enter the choice to edit :\n");
                int choice;
                printf("1.Edit Name\n2.Mobile Number\n3.Email Id\n4.Address\n");
                scanf("%d",&choice);
                switch(choice)
                {
                    case 1 : printf("Enter the name : ");
                             label_4 : scanf(" %[^\n]",buffer);
                             for(int i=0;buffer[i]!='\0';i++)
                            {
                                if(isalpha(buffer[i])==0)
                                {
                                    printf("Error enter valid name!!\n");
                                    goto label_4;
                                }
                            }
                            strcpy(contacts.name,buffer);
                            break;
                    case 2 : printf("Enter the mobile number : ");
                            label_5 : scanf(" %[^\n]",buffer);
                             if(validate_mobile(buffer,0))
                                strcpy(contacts.mobile,buffer);
                             else
                             {
                                if(revalidation())
                                {
                                    printf("Enter the mobile Number : \n");
                                    goto label_5;
                                }
                                else
                                    return;
                             } 
                            break;
                    case 3 : printf("Enter the email : ");
                            label_6 : scanf(" %[^\n]",buffer);
                             if(validate_email(buffer))
                                strcpy(contacts.email,buffer);
                             else
                            {
                                if(revalidation())
                                    goto label_6;
                                else
                                    return;
                            }
                            break;
                    case 4 : printf("Enter the address : ");
                             scanf(" %[^\n]",contacts.address);
                             break;
                    default : printf("Invalid choice\n");
                }

            }
        }
        fprintf(ftemp,"%s,%s,%s,%s\n",contacts.name,contacts.mobile,contacts.email,contacts.address);
    }
    if(flag==0)
        printf("Error contact not found!!\n");
    else
        printf("Contact Updated Successfully\n");
    fclose(fdata);
    fclose(ftemp);
    fdata=fopen("data.csv","w");
    ftemp=fopen("temp.csv","r");
    copy(fdata,ftemp);
    fclose(fdata);
    fclose(ftemp);
}
//_______________________________________________________________________________________________________
void delete_contact()
{
    system("clear");
    FILE *fdata = fopen("data.csv","r");
    FILE *ftemp = fopen("temp.csv","w");
    int count = 0;
    fscanf(fdata,"%d\n",&count);
    struct data contacts;
    char search[20];
    printf("Enter the name to delete : ");
    scanf(" %[^\n]",search);
    int name_occ=0;
    for(int i=0;i<count;i++)
    {
        fscanf(fdata,"%[^,],%[^,],%[^,],%[^\n]\n",contacts.name,contacts.mobile,contacts.email,contacts.address);
        if((strcmp(search,contacts.name))==0)
            name_occ=name_occ+1;
        
    }
    if(name_occ==0)
    {
        printf("Error contact not found!!!\n");
        return;
    }
    char find_mobile[11];
    char buffer[11];
    rewind(fdata);
    fscanf(fdata,"%d\n",&count);
    fprintf(ftemp,"%d\n",count-1);
    if(name_occ>=2)
    {
        printf("Multiple contacts found for same name.enter mobile to confirm: ");
        label_7 : scanf(" %[^\n]",buffer);
        if(validate_mobile(buffer,1))
            strcpy(find_mobile,buffer);
        else
        {
            if(revalidation())
                goto label_7;
            else
                return;
        }      
    }
    int flag=0;
    for (int i=0;i<count;i++) 
    {
        fscanf(fdata, "%[^,],%[^,],%[^,],%[^\n]\n",contacts.name,contacts.mobile,contacts.email,contacts.address);
        if (strcmp(search,contacts.name)==0) 
        {
            if (name_occ==1 || strcmp(contacts.mobile,find_mobile)==0) 
            {
                flag=1;
                continue; 
            }
        }
        fprintf(ftemp,"%s,%s,%s,%s\n",contacts.name,contacts.mobile,contacts.email,contacts.address);
    }
    if(flag==0)
        printf("Error contact not found\n");

    else 
        printf("Contact Deleted Successfully\n");
    
    fclose(fdata);
    fclose(ftemp);
    fdata=fopen("data.csv","w");
    ftemp=fopen("temp.csv","r");
    copy(fdata,ftemp);
    fclose(fdata);
    fclose(ftemp);
}

int revalidation()
{
    printf("Do you want to enter the data again (y/n) ?\n");
    char ch;
    scanf(" %c",&ch);
    if(ch == 'y' || ch == 'Y')
        return 1;
    else
        return 0;
}
