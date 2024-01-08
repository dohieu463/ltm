#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#define BUFF_SIZE 8192 

void print(int msg);  // function that processes messages received from the server

int main (int argc , char *argv[]){
	if(argc != 3)
	    perror("You must run program with port number !");
	int port = atoi(argv[2]);
	if(port == -1)
	    perror("Invalid port number ");
	    
	int client_sock ; 
	char buff[BUFF_SIZE];
	struct sockaddr_in server_addr; // server's address information
	
	int msg_len , bytes_sent , bytes_received ; 
	
	// Step 1 : construct socket 
	client_sock = socket(AF_INET , SOCK_STREAM , 0);
	// step 2 : specify cerver address 
	server_addr.sin_family = AF_INET ; 
	server_addr.sin_port = htons(port);
	server_addr.sin_addr.s_addr = inet_addr(argv[1]);
	// step 3 : request to connect server 
	if(connect(client_sock,(struct sockaddr*)&server_addr,sizeof(struct sockaddr)) < 0){
		printf("\nError!can not connect to server! Client exit imediately!");
		return 0 ; 
	} 
	// step 4 : communicate with server 
	int menu ; 
	do
    {
        // menu 
        printf("\n\n***************************************\n");
        printf("user management program \n");
        printf("-------------------------------------------\n");
        printf("  functions : \n");
        printf("1 . log in \n");
        printf("2 . sign up \n");
        printf("3 . log out \n");
        printf("4 . creat new project \n");
        printf("5 . List of participated projects\n");
        printf("6 . Add member to My project \n");
        printf("7 . Job Reviews \n");
        printf("8 . Find Project \n");
        printf("9 . View project details \n");
        printf("10. creat new task \n");
		printf("11. list task of project \n");
		printf("12. Update task \n"); 
		printf("13. Set Task for Member \n"); 
        printf("14 . exit the program \n");
        printf("*******************************************\n");
        printf("Please choose 1 of the 14 functions above : \n");
        
        // function selection
        scanf("%d", &menu);
        switch(menu)  
        {
        	case 1 : // if you choose the login function
        	{
        	
				printf("you have selected the login function \n");
                printf("Enter username : \n");
			    char name[BUFF_SIZE];
			    scanf("%s", name);
				  
			    // Send a login request message
                char msg[BUFF_SIZE];  //msg contains the message to be sent
                memset(msg,'\0',BUFF_SIZE);
                strcat(msg,"LOGIN ");
                strcat(msg,name);
                
                // nhap pass
                 printf("Enter password : \n");
                char pass[BUFF_SIZE];
			    scanf("%s", pass);
			    strcat(msg," ");
				strcat(msg, pass);
				// send message Login  
                bytes_sent = send(client_sock,msg,strlen(msg),0);
               
                //receive return messages from the server 
                bytes_received = recv(client_sock,buff , BUFF_SIZE , 0);
			    printf("\n%s\n", buff);
			    int num = atoi(buff); // Converts the received message from a character string to an integer
			    print(num); // print out the message
				 
			    break ;  
		    }
			   
			case 2 : // If you choose the post function
			{
				
			    printf("you have selected the SignUp function \n");
                printf("Enter username : \n");
			    char name[BUFF_SIZE];
			    scanf("%s", name);
			    // Send a SignUp request message
                char msg[BUFF_SIZE];  //msg contains the message to be sent
                memset(msg,'\0',BUFF_SIZE);
                strcat(msg,"CREAT_ACCOUNT ");
                strcat(msg,name);
                // nhap pass
                printf("Enter password : \n");
                char pass[BUFF_SIZE];
			    scanf("%s", pass);
			    strcat(msg," ");
				strcat(msg, pass);
				// send message SignUp
                bytes_sent = send(client_sock,msg,strlen(msg),0);
               
				//receive return messages from the server 
                bytes_received = recv(client_sock,buff , BUFF_SIZE , 0);
			    printf("\n%s\n", buff);
			    int num = atoi(buff); // Converts the received message from a character string to an integer
			    print(num);           // print out the message
			    
			    break ;  
			}
			case 3 : // If you choose the log out function
			{
				
				char msg[BUFF_SIZE];  //msg contains the message to be sent
                memset(msg,'\0',BUFF_SIZE);
                strcat(msg,"LOG_OUT ");
                // send message LogOut
                bytes_sent = send(client_sock,msg,strlen(msg),0);
                
				//receive return messages from the server 
                bytes_received = recv(client_sock,buff , BUFF_SIZE , 0);
                char *buff_num = strtok(buff , " ");
			    printf("\n%s\n", buff);
			    int num = atoi(buff_num); // Converts the received message from a character string to an integer
			    print(num);           // print out the message
				break ;  
			} 
			case 4 : 
			{
				char msg[BUFF_SIZE];
				memset(msg,'\0',BUFF_SIZE);
                strcat(msg,"MKPJ ");
                printf("Enter project name : \n");
			    char name[100];
			    scanf("%s", name);
			    strcat(msg,name);
			    strcat(msg," ");
				printf("Enter endTime : \n");
			    char endTime[100];
			    scanf("%s", endTime);
			    strcat(msg,endTime);                                
			    // send message 
                bytes_sent = send(client_sock,msg,strlen(msg),0);
               
				//receive return messages from the server 
                bytes_received = recv(client_sock,buff , BUFF_SIZE , 0);
			    printf("\n%s\n", buff);
			    int num = atoi(buff); // Converts the received message from a character string to an integer
			    print(num);           // print out the message
			    break ; 	
			} 
			case 5 : 
			{
				char msg[BUFF_SIZE];
				memset(msg,'\0',BUFF_SIZE);
                strcat(msg,"LSPJ ");
                // send message 
                bytes_sent = send(client_sock,msg,strlen(msg),0);
                //receive return messages from the server 
                bytes_received = recv(client_sock,buff , BUFF_SIZE , 0);
			    printf("\n%s\n", buff);
			    int num = atoi(buff); // Converts the received message from a character string to an integer
			    print(num);           // print out the message
			    break ; 
			}
			case 6 : 
			{
				char msg[BUFF_SIZE];
				memset(msg,'\0',BUFF_SIZE);
                strcat(msg,"ADDMEMPJ ");
                printf("enter project id : ");
                char id[100] ; 
                scanf("%s", id);
                strcat(msg,id);
                strcat(msg," ");
                char name[100] ; 
                printf("enter member name : ");
                scanf("%s" ,name);
                strcat(msg,name);
                // send message 
                bytes_sent = send(client_sock,msg,strlen(msg),0);
                //receive return messages from the server 
                bytes_received = recv(client_sock,buff , BUFF_SIZE , 0);
			    printf("\n%s\n", buff);
			    int num = atoi(buff); // Converts the received message from a character string to an integer
			    print(num);           // print out the message
			    break ; 
			} 
			case 7 : 
			{
				char msg[BUFF_SIZE];
				memset(msg,'\0',BUFF_SIZE);
                strcat(msg,"RVTASK ");
                printf("enter project id : ");
                char pj_id[100] ; 
                scanf("%s", pj_id);
                strcat(msg,pj_id);
                strcat(msg," ");
                char task_id[100] ; 
                printf("enter member name : ");
                scanf("%s" ,task_id);
                strcat(msg,task_id);
                // send message 
                bytes_sent = send(client_sock,msg,strlen(msg),0);
                //receive return messages from the server 
                bytes_received = recv(client_sock,buff , BUFF_SIZE , 0);
			    printf("\n%s\n", buff);
			    int num = atoi(buff); // Converts the received message from a character string to an integer
			    print(num);           // print out the message
			    break ; 
			} 
			case 8 : 
			{
				char msg[BUFF_SIZE];
				memset(msg,'\0',BUFF_SIZE);
                strcat(msg,"FINDPJ ");
                printf("enter project name  :");
				char name[100];
				scanf("%s", name);
				strcat(msg,name);
                // send message 
                bytes_sent = send(client_sock,msg,strlen(msg),0);
                //receive return messages from the server 
                bytes_received = recv(client_sock,buff , BUFF_SIZE , 0);
			    printf("\n%s\n", buff);
			    int num = atoi(buff); // Converts the received message from a character string to an integer
			    print(num);           // print out the message
			    break ; 
			} 
			case 9 : 
			{
				char msg[BUFF_SIZE];
				memset(msg,'\0',BUFF_SIZE);
                strcat(msg,"CHKPJ ");
                printf("enter project id :");
				char id[100];
				scanf("%s", id);
				strcat(msg,id);
                // send message 
                bytes_sent = send(client_sock,msg,strlen(msg),0);
                //receive return messages from the server 
                bytes_received = recv(client_sock,buff , BUFF_SIZE , 0);
			    printf("\n%s\n", buff);
			    int num = atoi(buff); // Converts the received message from a character string to an integer
			    print(num);           // print out the message
			    break ; 
			} 
			case 10 : 
			{
				char msg[BUFF_SIZE];
				memset(msg,'\0',BUFF_SIZE);
                strcat(msg,"MKTASK ");
                char id[100];
				scanf("%s", id);
				strcat(msg,id);
				strcat(msg," ");
                printf("Enter task name : \n");
			    char name[100];
			    scanf("%s", name);
			    strcat(msg,name);
			    strcat(msg," ");
				printf("Enter endTime : \n");
			    char endTime[100];
			    scanf("%s", endTime);
			    strcat(msg,endTime);                                
			    // send message 
                bytes_sent = send(client_sock,msg,strlen(msg),0);
               
				//receive return messages from the server 
                bytes_received = recv(client_sock,buff , BUFF_SIZE , 0);
			    printf("\n%s\n", buff);
			    int num = atoi(buff); // Converts the received message from a character string to an integer
			    print(num);           // print out the message
			    break ; 	
			} 
			case 11 : 
			{
				char msg[BUFF_SIZE];
				memset(msg,'\0',BUFF_SIZE);
                strcat(msg,"LSTASK ");
                char id[100];
				scanf("%s", id);
				strcat(msg,id);
				                             
			    // send message 
                bytes_sent = send(client_sock,msg,strlen(msg),0);
               
				//receive return messages from the server 
                bytes_received = recv(client_sock,buff , BUFF_SIZE , 0);
			    printf("\n%s\n", buff);
			    int num = atoi(buff); // Converts the received message from a character string to an integer
			    print(num);           // print out the message
			    break ; 
			}
			case 12 : 
			{
				char msg[BUFF_SIZE];
				memset(msg,'\0',BUFF_SIZE);
                strcat(msg,"UDPTASK ");
                char pj_id[100];
				scanf("%s", pj_id);
				strcat(msg,pj_id);
				strcat(msg," ");
                printf("Enter mem name : \n");
			    char name[100];
			    scanf("%s", name);
			    strcat(msg,name);
			    strcat(msg," ");
				printf("Enter Task Id : \n");
			    char Task_id[100];
			    scanf("%s", Task_id);
			    strcat(msg,Task_id);                                
			    // send message 
                bytes_sent = send(client_sock,msg,strlen(msg),0);
               
				//receive return messages from the server 
                bytes_received = recv(client_sock,buff , BUFF_SIZE , 0);
			    printf("\n%s\n", buff);
			    int num = atoi(buff); // Converts the received message from a character string to an integer
			    print(num);           // print out the message
			    break ; 	
			} 
			case 13 : 
			{
				char msg[BUFF_SIZE];
				memset(msg,'\0',BUFF_SIZE);
                strcat(msg,"SETTASK ");
                char pj_id[100];
				scanf("%s", pj_id);
				strcat(msg,pj_id);
				strcat(msg," ");
                printf("Enter mem name : \n");
			    char name[100];
			    scanf("%s", name);
			    strcat(msg,name);
			    strcat(msg," ");
				printf("Enter Task Id : \n");
			    char Task_id[100];
			    scanf("%s", Task_id);
			    strcat(msg,Task_id);                                
			    // send message 
                bytes_sent = send(client_sock,msg,strlen(msg),0);
               
				//receive return messages from the server 
                bytes_received = recv(client_sock,buff , BUFF_SIZE , 0);
			    printf("\n%s\n", buff);
			    int num = atoi(buff); // Converts the received message from a character string to an integer
			    print(num);           // print out the message
			    break ; 	
			} 
			case 14 : //If you choose to exit the program
            {
                printf("you have exited the program \n");
                break ; 
            }
            
            default : // if invalid functions are selected
            {
                printf("Please select functions from 1 to 4\n");
                break ; 
            }
		}
    }while(menu != 14);
    close(client_sock);
    return 0 ; 
}
/**
*@function : print : function that processes messages received from the server
*@param : msg(int) :   message code received
*@return : void  
**/
void print(int msg){
	switch(msg ){
		case 1111 : 
		{
			printf("ten tai khoan da ton tai \n");
			break ;  
		} 
		case 1100 : 
		{
			printf("dang ky tai khoan thanh cong  \n");
			break;
		}
		case 1214 : 
		{
			printf("ban dang dang nhap bang 1 tai khoan khac \n");
			break;
		}
		case 1213 : 
		{
			printf("khong ton tai ten tai khoan \n");
			break;
		}
		case 1212 : 
		{
			printf("sai mat khau \n");
			break;
		}
		case 1211 : 
		{
			printf("tai khoan bi khoa \n");
			break;
		}
		case 1210 : 
		{
			printf("tai khoan dang duoc dang nhap o 1 phien khac \n");
			break;
		}
		case 1200 : 
		{
			printf("dang nhap thanh cong  \n");
			break;
		}
		case 1300 : 
		{
			printf("tai khoan chua dang nhap \n");
			break;
		}
		case 1301 : 
		{
			printf("dang xuat thanh cong \n");
			break;
		}
		case 2111 : 
		{
			printf("thong tin khong dung dinh dang \n");
			break;
		}
		case 2112 : 
		{
			printf("khong day du thong tin \n");
			break;
		}
		case 2113 : 
		{
			printf("khong co quyen them \n");
			break;
		}
		case 2110 : 
		{
			printf("thoi diem khong hop le \n");
			break;
		}
		case 2100 : 
		{
			printf("tao cong viec thanh cong \n");
			break;
		}
		case 2212 : 
		{
			printf("khong tim thay du an \n");
			break;
		}
		case 2211 : 
		{
			printf("khong co cong viec trong du an \n");
			break;
		}
		case 2210 : 
		{
			printf("ko co quyen truy suat cong viec trong du an \n");
			break;
		}
		case 2200 : 
		{
			printf("tra ve danh sach thanh cong \n");
			break;
		}
		case 2312 : 
		{
			printf("ko tim thanh cong viec/thanh vien \n");
			break;
		}
		case 2311 : 
		{
			printf("cong viec da hoan thanh/het han \n");
			break;
		}
		case 2310 : 
		{
			printf("khong co quyen chinh sua \n");
			break;
		}
		case 2300 : 
		{
			printf("sua thong tin thanh cong\n");
			break;
		}
		case 2413 : 
		{
			printf("thanh vien da duoc gan cho cong viec nay \n");
			break;
		}
		case 2412 : 
		{
			printf("ko tim thay cong viec/thanh vien \n");
			break;
		}
		case 2411 : 
		{
			printf("cong viec da hoan thanh/het han \n");
			break;
		}
		case 2410 : 
		{
			printf("ko co quyen chinh sua \n");
			break;
		}
		case 2400 : 
		{
			printf("sua thong tin thanh cong \n");
			break;
		}
		case 3111 : 
		{
			printf("thong tin khong dung dinh dang \n");
			break;
		}
		case 3112 : 
		{
			printf("khong day du thong tin \n");
			break;
		}
		case 3110 : 
		{
			printf("thoi diem khong hop le \n");
			break;
		}
		case 3100 : 
		{
			printf("tao project thanh cong \n");
			break;
		}
		case 3211 : 
		{
			printf("khong tim thay du an\n");
			break;
		}
		case 3210 : 
		{
			printf("nguoi dung khong hop le \n");
			break;
		}
		case 3200 : 
		{
			printf("tra ve danh sach thanh cong \n");
			break;
		}
		case 3312 : 
		{
			printf("khong tim thay du an \n");
			break;
		}
		case 3311 : 
		{
			printf("khong the them nguoi dung \n");
			break;
		}
		case 3310 : 
		{
			printf("khong co quyen chinh sua \n");
			break;
		}
		case 3300 : 
		{
			printf("sua thong tin thanh cong \n");
			break;
		}
		case 3412 : 
		{
			printf("khong tim thay cong viec/du an/ thanh vien \n");
			break;
		}
		case 3411 : 
		{
			printf("khong co quyen nhan xet \n");
			break;
		}
		case 3410 : 
		{
			printf("thoi diem khong hop le \n");
			break;
		}
		case 3400 : 
		{
			printf("sua thong tin thanh cong \n");
			break;
		}
		case 3503 : 
		{
			printf("ten khong hop le \n");
			break;
		}
		case 3502 : 
		{
			printf("khong tim thay du an \n");
			break;
		}case 3500 : 
		{
			printf("tra ve danh sach thanh cong \n");
			break;
		}
		case 3611 : 
		{
			printf("khong co quyen xem du an \n");
			break;
		}
		case 3610 : 
		{
			printf("khong tim thay nguoi dung/ du an \n");
			break;
		}
		case 3600 : 
		{
			printf("tra ve thong tin thanh cong \n");
			break;
		}
		
		default : 
		{
			printf("message cannot be determined in the above cases \n"); 
			break ;  
		} 
	}
} 

