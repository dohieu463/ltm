#include <stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<ctype.h>
#include<assert.h>
#include<pthread.h>
#include"account.c"
#include"project.c"
#include"task.c" 
#include"protocol.c"
#define BACK_LOG 2 // number of allowed connections 
#define BUFF_SIZE 1024 

void handle_creat_account(int client_sockfd, Session *session);
void *handle_client(void *arg);
void handle_login(int client_sockfd, Session *session);
void handle_LogOut(int client_sockfd , Session *session);
void handle_CreatNewProject(int client_sockfd,Session *session);
void handle_LSPJ(int client_sockfd,Session *session);
void handle_Add_Member(int client_sockfd,Session * session);
void handle_UDPTASK(int client_sockfd,Session * session);
void handle_SETTASK(int client_sockfd,Session * session);
void handle_LSTASK(int client_sockfd,Session *session);
void handle_MKTASK(int client_sockfd,Session *session);
void handle_CHKPJ(int client_sockfd,Session * session);
void handle_FINDPJ(int client_sockfd,Session * session);
void handle_RVTASK(int client_sockfd,Session * session);
//Account **acc ; 
//Project **List_Project ;  
pthread_mutex_t clients_mutex;

int main (int argc, char* argv[])
{
	int menu ; 
	int listenfd , *connfd ; 
    root_acc = createAccountList();
    root_pj = createProjectList(); 
    
	memset(root_pj, '\0', sizeof(root_pj));
	if (*root_pj == NULL) printf("a\n");
		else printf("b\n");
	root_acc = readAccount(*root_acc);
    // Khoi tao mutex
    pthread_mutex_init(&clients_mutex, NULL);
	
	if(argc != 2)
		perror("must enter 2 input parameters \n");
	
	int port = atoi(argv[1]);
	
	if(port == -1)
		perror("port is incorrect !\n");
	
	// step 1 : construct a TCP socket to listen connection request

	struct sockaddr_in server ; // server's address information 
	struct sockaddr_in *client ; // client's address information
	pthread_t tid  ; 
	int sin_size; 
	
	if((listenfd = socket(AF_INET,SOCK_STREAM,0))==-1){
		printf("socket() error \n");
		return 0 ; 
	}
	// step 2 : bind address to socket 
	bzero(&server,sizeof(server));
	server.sin_family = AF_INET ; 
	server.sin_port = htons(port);
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	
	// calls bind() 
	if(bind(listenfd,(struct sockaddr*)&server,sizeof(server))==-1){
		perror("\nError : ");
		return 0 ; 
	}
	// step 3 : listen request from client 
	if(listen(listenfd,BACK_LOG) == -1){// calls listen()
	   perror("\nError : ");
	   return 0 ; 
	}  
	
	sin_size = sizeof(struct sockaddr_in);
	client = malloc(sin_size);
    // step 4 : communicate with client 
	
	while(1){
		connfd = malloc(sizeof(int));
		if((*connfd = accept(listenfd,(struct sockaddr*)client,&sin_size)) == -1){
			perror("\nError : ");
		}
		printf("\n you got a connection from %s\n",inet_ntoa((*client).sin_addr));// print client's IP
		
		// tao luong de xu ly yeu cau cua khach hang  
        pthread_create(&tid,NULL,handle_client,connfd);
	} 
	// close socket 
	close(listenfd);
	
	// X�a mutex
    pthread_mutex_destroy(&clients_mutex);
	return 0 ; 
}
/**
ham handle_client xu ly yeu cau cua khach hang  
**/
void *handle_client(void *arg){
	int client_sockfd =  *((int *) arg); 
	int bytes_sent , received_bytes ;

	free(arg);
	
	// Cap phat bo nho cho session 
    Session *session = malloc(sizeof(Session));
	session->account = NULL ; 
	 
	session->socket = client_sockfd ; 
	pthread_detach(pthread_self());
	
	while(1){
	
	    char msg_rev[BUFF_SIZE];
		memset(msg_rev,'\0',strlen(msg_rev));
		// receive mess from client 
		received_bytes = recv(client_sockfd,msg_rev,BUFF_SIZE,0);
		
		if(received_bytes < 0){
			perror("Error: ");
			if (session->account != NULL)
                session->account->IsLogIn = 0;
            break;  
		}
		else if( received_bytes == 0){
			printf("Connection closed.");
            if (session->account != NULL)
			    session->account->IsLogIn = 0 ;  
			break;  
		}
		else{
			msg_rev[received_bytes] = '\0';
		    printf("\n%s\n",msg_rev );
		
		    char *request_type ; 
		    request_type = strtok(msg_rev , " ");
		    if(strcmp(request_type,"LOGIN") == 0){
			   // goi ham xac thuc nguoi dung 
			   handle_login(client_sockfd, session);
		    }
		    else if(strcmp(request_type,"CREAT_ACCOUNT") == 0){
			   // goi ham tao tai khoan  
			   handle_creat_account(client_sockfd, session);
		    }
		    else if(strcmp(request_type,"LOG_OUT") == 0){
			   // goi ham logout
			   handle_LogOut(client_sockfd, session);
		    }
		    else if(strcmp(request_type,"MKPJ") == 0){
			   // goi ham logout
			   handle_CreatNewProject(client_sockfd, session);
		    }
		    else if(strcmp(request_type,"LSPJ") == 0){
			   // goi ham handle_LSPJ 
			   handle_LSPJ(client_sockfd, session);
		    }
		    else if(strcmp(request_type,"ADDMEMPJ") == 0){
			   // goi ham handle_LSPJ 
			   handle_Add_Member(client_sockfd, session);
		    }
		    /*
		    else if(strcmp(request_type,"RVTASK") == 0){
			   // goi ham handle_LSPJ 
			   handle_RVTASK(client_sockfd, session);
		    }
		    */
		    else if(strcmp(request_type,"FINDPJ") == 0){
			   // goi ham handle_LSPJ 
			   handle_FINDPJ(client_sockfd, session);
		    }
		    else if(strcmp(request_type,"CHKPJ") == 0){
			   // goi ham handle_CHKPJ 
			   handle_CHKPJ(client_sockfd, session);
		    }
		    else if(strcmp(request_type,"MKTASK") == 0){
			   // goi ham handle_CHKPJ 
			   handle_MKTASK(client_sockfd, session);
		    }
		    else if(strcmp(request_type,"LSTASK") == 0){
			   // goi ham handle_CHKPJ 
			   handle_LSTASK(client_sockfd, session);
		    }
		    else if(strcmp(request_type,"UDPTASK") == 0){
			   // goi ham handle_CHKPJ 
			   handle_UDPTASK(client_sockfd, session);
		    }
		    else if(strcmp(request_type,"SETTASK") == 0){
			   // goi ham handle_CHKPJ 
			   handle_SETTASK(client_sockfd, session);
		    }
		    else{
			   // 300 : khong xac dinh duoc yeu cau    
               received_bytes = send(client_sockfd,"300",sizeof("300"),0); 
		       break ;  
		    } 
		} 
	}
	free(session); 
	close(client_sockfd); 
} 

void handle_UDPTASK(int client_sockfd,Session * session){
	int bytes_sent , received_bytes ;
    pthread_mutex_lock(&clients_mutex);
    
    if(session->account == NULL){
		received_bytes = send(client_sockfd,"1300",sizeof("1300"),0); 
	}
	else{
		char *Pj_id , *mem_name , *Task_id; 
		Pj_id = strtok(NULL , " "); // lay projname nhap vao  
        mem_name = strtok(NULL, " "); // lay endTime nhap vao
        Task_id = strtok(NULL, " ");
		int pj_id = atoi(Pj_id);
		int task_id = atoi(Task_id); 
		char *mess_send = updtask(pj_id,mem_name, task_id, session->account);
		mess_send[strlen(mess_send)] = '\0';

		received_bytes = send(client_sockfd , mess_send , strlen(mess_send) , 0); 
	}
	pthread_mutex_unlock(&clients_mutex);
}

void handle_SETTASK(int client_sockfd,Session * session){
	int bytes_sent , received_bytes ;
    pthread_mutex_lock(&clients_mutex);
    
    if(session->account == NULL){
		received_bytes = send(client_sockfd,"1300",sizeof("1300"),0); 
	}
	else{
		char *Pj_id , *mem_name , *Task_id; 
		Pj_id = strtok(NULL , " "); // lay projname nhap vao  
        mem_name = strtok(NULL, " "); // lay endTime nhap vao
        Task_id = strtok(NULL, " ");
		int pj_id = atoi(Pj_id);
		int task_id = atoi(Task_id); 
		char *mess_send = updtask(pj_id,mem_name, task_id, session->account);
		mess_send[strlen(mess_send)] = '\0'; 
		
		received_bytes = send(client_sockfd , mess_send , strlen(mess_send) , 0);
	}
	pthread_mutex_unlock(&clients_mutex);
}

void handle_LSTASK(int client_sockfd,Session *session){
	int bytes_sent , received_bytes ;
    pthread_mutex_lock(&clients_mutex);
	if(session->account == NULL){
		received_bytes = send(client_sockfd,"1300",sizeof("1300"),0); 
	}
	else{
		char *Pj_id;
		Pj_id = strtok(NULL , " ");  
	    int pj_id = atoi(Pj_id);
        char *mess_send = lstask(pj_id, session->account);
		mess_send[strlen(mess_send)] = '\0';
		
		received_bytes = send(client_sockfd , mess_send , strlen(mess_send) , 0);
	} 
	pthread_mutex_unlock(&clients_mutex);
}

void handle_MKTASK(int client_sockfd,Session *session){
	int bytes_sent , received_bytes ;
    pthread_mutex_lock(&clients_mutex);
	if(session->account == NULL){
		received_bytes = send(client_sockfd,"1300",sizeof("1300"),0); 
	}
	else{
		char *Pj_id , *taskName , *endTime ;
		Pj_id = strtok(NULL , " ");  
		taskName = strtok(NULL , " "); // lay projname nhap vao  
        endTime = strtok(NULL, " "); // lay endTime nhap vao
        int pj_id = atoi(Pj_id);
        char *mess_send = mk_task(pj_id,taskName, endTime,session->account);
		mess_send[strlen(mess_send)] = '\0';
		
		received_bytes = send(client_sockfd , mess_send , strlen(mess_send) , 0);
	} 
	pthread_mutex_unlock(&clients_mutex);
}

void handle_CHKPJ(int client_sockfd,Session * session){
    int bytes_sent , received_bytes ;
    pthread_mutex_lock(&clients_mutex);
    
    if(session->account == NULL){
		received_bytes = send(client_sockfd,"1300",sizeof("1300"),0); 
	}
	else{
		char *Pj_id; 
		Pj_id = strtok(NULL , " "); // lay proid nhap vao
		int pj_id = atoi(Pj_id);  
		char *mess_send = chk_pj(session->account,pj_id);
		mess_send[strlen(mess_send)] = '\0';
		
		received_bytes = send(client_sockfd , mess_send , strlen(mess_send) , 0);
	}
	pthread_mutex_unlock(&clients_mutex);
}

void handle_FINDPJ(int client_sockfd,Session * session){
    int bytes_sent , received_bytes ;
    pthread_mutex_lock(&clients_mutex);
    
    if(session->account == NULL){
		received_bytes = send(client_sockfd,"1300",sizeof("1300"),0); 
	}
	else{
		char *Pj_name; 
		Pj_name = strtok(NULL , " "); // lay projname nhap vao  
		char *mess_send = find_pj(Pj_name);
		mess_send[strlen(mess_send)] = '\0';
		
		received_bytes = send(client_sockfd , mess_send , strlen(mess_send) , 0);
	}
	pthread_mutex_unlock(&clients_mutex);
}
/*
void handle_RVTASK(int client_sockfd,Session * session){
    int bytes_sent , received_bytes ;
    pthread_mutex_lock(&clients_mutex);
    
    if(session->account == NULL){
		received_bytes = send(client_sockfd,"1300",sizeof("1300"),0); 
        pthread_mutex_unlock(&clients_mutex);
	}
	else{
		char *Pj_id , *Task_id; 
		Pj_id = strtok(NULL , " "); // lay projname nhap vao  
        Task_id = strtok(NULL, " "); // lay endTime nhap vao
        int pj_id = atoi(Pj_id);
		int task_id = atoi(Task_id); 
		char *mess_send = addmem_pj(session->account, pj_id, mem_name);
		received_bytes = send(client_sockfd , mess_send , strlen(mess_send) , 0); 
        pthread_mutex_unlock(&clients_mutex); 
	}
}
*/
void handle_Add_Member(int client_sockfd,Session * session){
	int bytes_sent , received_bytes ;
    pthread_mutex_lock(&clients_mutex);
    
    if(session->account == NULL){
		received_bytes = send(client_sockfd,"1300",sizeof("1300"),0); 
	}
	else{
		char *Pj_id , *mem_name; 
		Pj_id = strtok(NULL , " "); // lay projname nhap vao  
        mem_name = strtok(NULL, " "); // lay endTime nhap vao
        int pj_id = atoi(Pj_id); 
		char *mess_send = addmem_pj(session->account, pj_id, mem_name);
		mess_send[strlen(mess_send)] = '\0';
		
		received_bytes = send(client_sockfd , mess_send , strlen(mess_send) , 0);
	}
	pthread_mutex_unlock(&clients_mutex);
}

void handle_LSPJ(int client_sockfd,Session *session){
	int bytes_sent , received_bytes ;
    pthread_mutex_lock(&clients_mutex);
	
	if(session->account == NULL){
		received_bytes = send(client_sockfd,"1300",sizeof("1300"),0); 
	}
	else{
		
		char *mess_send = ls_pj(session->account);
		printf("%s\n", mess_send);
		mess_send[strlen(mess_send)] = '\0';
		
		received_bytes = send(client_sockfd , mess_send , strlen(mess_send) , 0);
	} 
	pthread_mutex_unlock(&clients_mutex);
}

void handle_CreatNewProject(int client_sockfd,Session *session){
	int bytes_sent , received_bytes ;
    pthread_mutex_lock(&clients_mutex);
	if(session->account == NULL){
		received_bytes = send(client_sockfd,"1300",sizeof("1300"),0); 
	}
	else{
		if (root_pj == NULL) printf("a\n");
		else printf("b\n");
		printf("%s\n", printListProject(root_pj));
		char *projName , *endTime ;  
		projName = strtok(NULL , " "); // lay projname nhap vao  
        endTime = strtok(NULL, " "); // lay endTime nhap vao
        char *mess_send = mk_pj(projName,endTime,5,session->account);
		mess_send[strlen(mess_send)] = '\0';
		
		received_bytes = send(client_sockfd , mess_send , strlen(mess_send) , 0);
	} 
	pthread_mutex_unlock(&clients_mutex);
}

void handle_LogOut(int client_sockfd , Session *session){
	int bytes_sent , received_bytes ;

	pthread_mutex_lock(&clients_mutex);

	if(session->account == NULL){		
		// 1300 : log out that bai , tai khoan chua dang nhap     
        received_bytes = send(client_sockfd,"1300",sizeof("1300"),0); 
	}
	else{
		session->account->IsLogIn = 0 ; 
		session->account = NULL; 
	    // 1301 : log out thanh cong    
		received_bytes = send(client_sockfd,"1301",sizeof("1301"),0); 
	}
	pthread_mutex_unlock(&clients_mutex);
}

void handle_login(int client_sockfd , Session *session){
	char *username , *password;  
	int bytes_sent , received_bytes ;
	
	pthread_mutex_lock(&clients_mutex);
	
	Account *account; 
    if(session->account != NULL){		
		// 1214 : ban dang dang nhap tai khoan khac   
        received_bytes = send(client_sockfd,"1214",sizeof("1214"),0); 
	}
	else{    
	    username = strtok(NULL , " "); // lay username nhap vao  
        password = strtok(NULL, " "); // lay password nhap vao  
        // lay tai khoan tu co so du lieu 
	    account = findaccbyusername(root_acc,username);
	    if(account == NULL){
		    //1213 : khong ton tai tai khoan  
            received_bytes = send(client_sockfd,"1213",sizeof("1213"),0); 
	    }
	    else if(account->IsLogIn == 1){
	    	// 1210 : tai khoan dang duoc dang nhap o 1 phien khac    
            received_bytes = send(client_sockfd,"1210",sizeof("1210"),0); 
		}
		else if(account->accountStatus == 0){
			// 1211 : tai khoan dang bi khoa     
            received_bytes = send(client_sockfd,"1211",sizeof("1211"),0); 
		} 
	    else {
		   if(strcmp(account->password,password) == 0){
			//1200 : ban da dang nhap thanh cong   
			session->account = account; 
			session->account->IsLogIn = 1 ;  
            received_bytes = send(client_sockfd,"1200",sizeof("1200"),0); 
		    // open mutex 
		    }
		    else{
			   //212 : sai mat khau    
               received_bytes = send(client_sockfd,"1212",sizeof("1212"),0);
		    }
		} 
	}
	pthread_mutex_unlock(&clients_mutex);
}

void handle_creat_account(int client_sockfd ,  Session *session){
	char *username , *password;  
	int bytes_sent , received_bytes ;
	Account *account ; 
	
	pthread_mutex_lock(&clients_mutex);
	
	if(session->account != NULL){
		
		// 1214 : ban dang dang nhap bang 1 tai khoan khac      
        received_bytes = send(client_sockfd,"1214",sizeof("1214"),0); 
	}
	else{
	    username = strtok(NULL , " "); // lay username nhap vao  
    
        password = strtok(NULL, " "); // lay password nhap vao  
    
        // lay tai khoan tu co so du lieu 
	    account = findaccbyusername(root_acc,username);
	    if(account == NULL){
		    // tai khan khong ton tai , tao tai khoan moi 
		    Account *newAcc = newAccount(username , password , 1);
		    addAccount(root_acc,newAcc);
		    // save account into file 
			save_account(newAcc); 
		    // 1100 : dang ky tai khoan thanh cong    
            received_bytes = send(client_sockfd,"1100",sizeof("1100"),0); 
	    }
	    else{
		    // 1111 : tai khoan da ton tai    
            received_bytes = send(client_sockfd,"1111",sizeof("1111"),0); 
	    }
	}
	pthread_mutex_unlock(&clients_mutex);
} 

