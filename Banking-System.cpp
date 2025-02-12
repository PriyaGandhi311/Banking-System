#include<stdio.h>
#include<conio.h>
#include <bits/stdc++.h>
int main(){
system("cls");
float draw,dep,transfer;
char name[24];
float balance=5000;
int account,account1,account2;
int type;
int transaction=1;
printf("\n\t\t\t\tBANKING SYSTEM\n\n");
printf("\t\t\4 \4 \4 \4 \4 \4 \4 \4 \4 \4 \4 \4 \4 \4 \4 \4 \4 \4 \4 \4 \4 \4 \4 \4 \4\n");
printf("\t\t\4\t\t\t\t\t\t\4\n");
printf("\t\t\4\t\t\t\t\t\t\4\n");
printf("\t\t\4\tWELCOME TO OUR YAQ BANKING SYSTEM\t\t\4\n");
printf("\t\t\4\t\t\t\t\t\t\4\n");
printf("\t\t\4\t\t\t\t\t\t\4\n");
printf("\t\t\4 \4 \4 \4 \4 \4 \4 \4 \4 \4 \4 \4 \4 \4 \4 \4 \4 \4 \4 \4 \4 \4 \4 \4 \4\n");
printf("Enter you name: ");
scanf("%s",&name);
printf("\nPlease enter your account no.: ");
scanf("%d",&type);
while(transaction == 1)
{
int option;
printf("\nChoose what you want to do\n");
printf("1 - Balance Enquiry\n");
printf("2 - Deposit\n");
printf("3 - Withdraw\n");
printf("4 - transfer\n");
scanf("%d",&option);
switch(option)
{
case 1:
printf("\t\t\t*BALANCE ENQUIRY*\n\n");
printf("Your current balance is: %.2fUSD\n",balance);
break;
case 2:
printf("\n\t\t\t*DEPOSIT AMMOUNT*\n\n");
printf("How much money do you want to deposit:");
scanf("%f",&dep);
if(dep > 0 && dep<=20000)
{
printf("\nyour %.2fUSD deposited in your account.\n\n",dep);
balance+=dep;
}
else if(dep>20000)
{
printf("\nYou can't deposit that much ammount in one time.\n\n");
}
else
{
printf("\nInvalid deposit amount\n");
}
break;
case 3:
printf("\n\t\t*WITHDRAW AMMOUNT*\n\n");
printf("How much money do you want to withdraw:");
scanf("%f",&draw);
if(draw<=balance && draw<=20000)
{
printf("\nYou just withdraw %.2fUSD\n\n",draw);
balance-=draw;
}
else if(draw>20000)
{
printf("\nYou can't withdraw that much amount in one time.\n\n");
}
else
{
printf("\nYou dont have enough money\n\n");
}
break;
case 4:
printf("\t\t\tTRANSFER AMMOUNT \n\n");
printf("\n\t\tAccount You Want To Transfer: ");
scanf("%d",&account2);
printf("\nHow much ammount?: ");
scanf("%f",&transfer);
if(balance>=transfer)
{
printf("\nyour %.2fUSD successfully transfered\n\n",transfer);
balance-=transfer;
}
else
{printf("\nYou do not have sufficient balance\n\n");}
break;
default:
printf("Invalid transaction\n");
}
transaction=0;
while(transaction!=1 && transaction!=2)
{
printf("Do you want to do another transaction?\n");
printf("1. yes 2. no\n");
scanf("%d",&transaction);
if(transaction!=1 && transaction!=2)
{printf("Invalid no.\nchoose between 1 and 2 only\n");}}}
system("cls");
printf("\n\t\t\t    -----------------------");
printf("\n\t\t\t\tYAQ BANK LIMITED\n");
printf("\t\t\t    -----------------------\n\n");
printf("\t\t\tDate:3/5/16\t   Time:10:20 Am\n");
printf("\n\t\t\t\4 Your name: %s\n\n",name);
printf("\t\t\t\4 Your account no: %d\n\n",type);
if(dep >= 0 && dep < 20000)
{
printf("\t\t\t\4 You've deposited %.2fUSD\n",dep);
}
else
{printf("\t\t\t\4 You've deposited 0USD\n");}
if(draw>0 && draw<=20000 && draw<=balance)
{
printf("\t\t\t\4 You've withdraw %.2fUSD\n",draw);
}
else
{printf("\t\t\t\4 You've withdraw 0USD\n");}
if(transfer>0 && transfer<=20000 && transfer<=balance)
{
printf("\t\t\t\4 You've Transfered %.2fUSD\n",transfer);}
else
{printf("You've Transfered 0USD\n ");}
printf("\n\t\t\t\t    Thank you! \n");
printf("\t\t\t   Welcome to  Banking System\n");
getch();
return(0);
}