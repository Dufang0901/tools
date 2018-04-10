#define VERS "1.0"
#define MAXTHREAD 10

//#define DEBUG // <-commenter cette ligne desactive le debogage
//#define COULEUR // <-commenter cette ligne desactive les couleurs

/* Declaration des fichiers d'en-tete */
#include <string.h>//pour les fonctions strlen entre autre
#include <stdio.h>//pour les printf, scanf
#include <stdlib.h>//pour la fonction atoi()
#include <ctype.h>//pour la fonction isdigit()
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>//pour les threads
#include <fcntl.h>//pour les fichiers binaires
#include <sys/stat.h>//pour les fichiers binaires


/* Declaration des procedures */

int transmet (char* ordre, char* messageAvecEspace, char* reponse);//declaration de la fonction de transmition de message au serveur

int interpreteReponse (char* reponse);//interprete la reponse et renvoie le code reponse correspondant

char* interpreteCommande (char* commande, int taille);//interprete si besoin la commande avant de la trasmettre et renvoie la commande

void* thread (void* parametre);//processus associe a un thread

int getsize (char* fich);//retourne la taille du fichier en octets
void getPWD ();//met a jour la variable globale dossierCourant

void initialiseTransfert ();//envoie TYPE I puis PASV
void initialiseThread ();//met a zero les tableaux de threads
int trouveThreadLibre ();//renvoie le chiffre du premier thread libre
int trouveThreadPret ();//renvoie le chiffre du premier thread pret

/* Declaration des variables globales */
pthread_t threadID[MAXTHREAD];//on fixe le nombre maximum de thread
int socketTab[MAXTHREAD];//on fixe le nombre max de socket de données
int action[MAXTHREAD];//a effectuer par le thread: 0=afficher 1=ecrire un fichier 2=lire un fichier
int Ack[MAXTHREAD];//aquitement de fin de transfert: 0=non aquite 1=ACK realise
int threadPret[MAXTHREAD];//0=libre 1=pret 2=actif
int maxChaine = 250;//taille max d'une chaine d'un message
int maxMsg = 1449;//taille max du message reseau = taille zone donnée du packet tcp + '\0'
char nomFichier[250];//le nom du fichier qui devra etre traite par le prochain thread
char dossierCourant[250];//le dossier courant du SERVEUR
int tailleFichier;//la taille du prochain fichier a traiter dans le thread
int socketControle;//Socket de controle pour les commandes et réponses

/* Debut du programme */

int main (int nbPar, char * tabParam [])
{
	/* Declaration des variables */
	unsigned short portServ = 21; //port par defaut
	char urlServ[maxChaine];
	struct hostent *serveur; //declaration du pointeur sur le serveur
	#ifdef COULEUR
	system("tput setaf 0");//ecrire en noir
	#endif
	printf("-- Bienvenu sur notre CLIENT FTP version ");
	printf(VERS);
	printf(" --\n\n");

	/* Test du nombre de paramètres */

	if (nbPar==1) // Pas de parametre
	{
	printf("Veuillez rentrer l'adresse IP du serveur: ");
	scanf("%s",urlServ);
	}

	else if (nbPar==2)
	{
	strcpy(urlServ,tabParam[1]);
	}
	else if (nbPar==3)
	{
	/* Récupération du port de connexion au serveur */
	portServ = atoi(tabParam[2]);
	strcpy(urlServ,tabParam[1]);
	}
	else
	{
		printf("Paramètres incorrect\n");
		printf("Syntaxe:\t");
		printf(tabParam[0]);
		printf(" [serveur]\n\t\t");
		printf(tabParam[0]);
		printf(" <serveur> [port]\n");
		return 1;
	}

	printf("->Nous allons etablir une connexion a %s sur le port %i\n\n",urlServ,portServ);
	serveur = gethostbyname(urlServ); //on affecte le serveur
	if (serveur == NULL) //si serveur non valide
	{
		#ifdef COULEUR
		system("tput setaf 1");//ecrire en rouge
		#endif
		printf ("L'adresse [%s] est n'est pas valide\n", tabParam[1]);
		#ifdef COULEUR
		system("tput setaf 0");//ecrire en noir
		#endif
		return 1;//Arret brutal du programme
	}

	struct sockaddr_in servAddr;//declaration du socket serveur
	servAddr.sin_family = serveur->h_addrtype;//configuration du socket
	memcpy((char *) &servAddr.sin_addr.s_addr, serveur->h_addr_list[0], serveur->h_length);//On associe le socket serveur au serveur
	servAddr.sin_port = htons(portServ);//on parametre la socket pour le port voulu

    #ifdef DEBUG
        printf("\n>> DEBUG \n");
        printf("Socket serveur \n");
        printf("Adresse : %i \nPort : %i \n",ntohl(servAddr.sin_addr.s_addr), ntohs(servAddr.sin_port));
        printf(">> FIN DEBUG \n\n");
    #endif

	printf("Creation de la socket...\t\t");
	socketControle = socket(AF_INET, SOCK_STREAM, 0);//ouverture du socket

	if(socketControle<0) //si l'ouverture echoue
	{
		#ifdef COULEUR
		system("tput setaf 1");
		#endif
		printf("[ECHEC]\n");
		printf("Impossible d'ouvrir la socket\n");
		#ifdef COULEUR
		system("tput setaf 0");//ecrire en noir
		#endif
		return 1;//Arret brutal du programme
	}
	fflush(stdout);
	#ifdef COULEUR
	system("tput setaf 2");//ecrire en vert
	#endif
	printf("[  OK ]\n");
	#ifdef COULEUR
	system("tput setaf 0");//ecrire en noir
	#endif
	/*de meme on cree une adresse local*/
	struct sockaddr_in localAddr;
	localAddr.sin_family = AF_INET;
	localAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	localAddr.sin_port = htons(0);//le port utilisé sur le client peut etre quelconque

    #ifdef DEBUG
        printf("\n>> DEBUG \n");
        printf("Socket client \n");
        printf("Adresse : %i \nPort : %i \n", ntohl(localAddr.sin_addr.s_addr), ntohs(localAddr.sin_port));
        printf(">> FIN DEBUG \n\n");
    #endif

	int rc;//variable de resultat

	//connexion au serveur
	printf("Connexion au serveur...\t\t\t");
	fflush(stdout);
	rc = connect(socketControle,(struct sockaddr *) &servAddr,sizeof(servAddr));
	if(rc<0) //si la connexion echoue
	{
		#ifdef COULEUR
		system("tput setaf 1");
		#endif
		printf("[ECHEC]\n");
		printf("Connexion impossible\n");
		#ifdef COULEUR
		system("tput setaf 0");//ecrire en noir
		#endif
		return 1;//Arret brutal du programme
	}
	fflush(stdout);
	#ifdef COULEUR
	system("tput setaf 2");//ecrire en vert
	#endif
	printf("[  OK ]\n");
	#ifdef COULEUR
	system("tput setaf 0");//ecrire en noir
	#endif

	printf("Reception d'un flux FTP...\t\t");
	fflush(stdout);
	char reponse[maxMsg];//message a recevoir
	memset(reponse,0x0,maxMsg);//on prepare la zone memoire en la remplissant par le caractere '\0', utile car le pacquet recu ne comptiend pas ce caractere de fin de chaine
	rc = recv(socketControle, reponse, maxMsg-1, 0);
	if (rc<0)
	{
		#ifdef COULEUR
		system("tput setaf 1");
		#endif
		printf("[ECHEC]\n");
		printf("Impossible de recevoir des donnees\n");
		#ifdef COULEUR
		system("tput setaf 0");//ecrire en noir
		#endif
		return 1;//Arret brutal du programme
	}
	//interpretation de la reponse
	int codeRep = interpreteReponse (reponse);

	if (codeRep != 220)
	{
		#ifdef COULEUR
		system("tput setaf 1");
		#endif
		printf("[ECHEC]\n");
		printf("Impossible d'interpreter la reponse\n");
		#ifdef COULEUR
		system("tput setaf 0");//ecrire en noir
		#endif
		return 1;//Arret brutal du programme
	}
	fflush(stdout);
	#ifdef COULEUR
	system("tput setaf 2");//ecrire en vert
	#endif
	printf("[  OK ]\n\n");
	#ifdef COULEUR
	system("tput setaf 4");//ecrire en bleu clair
	#endif

    #ifdef DEBUG
        printf("\n>> DEBUG \n");
        printf("Code Reponse recu : %i \n",codeRep);
        printf(">> FIN DEBUG \n\n");
    #endif
	
	printf(reponse);//on affiche le datagramme reponse
	#ifdef COULEUR
	system("tput setaf 6");//ecrire en noir
	#endif

	//envoie du nom d'utilisateur
	char msg[maxChaine];//message a envoyer
	printf("Utilisateur: ");
	scanf("%s",msg);

	char* ordre;//commande a envoyer
	ordre = "USER";

	rc = transmet(ordre, msg, reponse);

	codeRep = interpreteReponse (reponse);
	
    #ifdef DEBUG
        printf("\n>> DEBUG \n");
        printf("Code Reponse recu : %i \n",codeRep);
        printf(">> FIN DEBUG \n\n");
    #endif
	
	if (codeRep != 331)
	{
		#ifdef COULEUR
		system("tput setaf 1");
		#endif
		printf("Erreur: reponse inatendue\n");
		#ifdef COULEUR
		system("tput setaf 0");//ecrire en noir
		#endif
		return 1;//Arret brutal du programme
	}

	//mot de pass
	printf("Mot de pass: ");
	scanf("%s",msg);
	ordre = "PASS";

	rc = transmet(ordre, msg, reponse);

	codeRep = interpreteReponse (reponse);

    #ifdef DEBUG
        printf("\n>> DEBUG \n");
        printf("Code Reponse recu : %i \n",codeRep);
        printf(">> FIN DEBUG \n\n");
    #endif
	
	if (codeRep != 230)
	{
		#ifdef COULEUR
		system("tput setaf 1");
		#endif
		printf("Identification refusée par le serveur\n");
		#ifdef COULEUR
		system("tput setaf 0");//ecrire en noir
		#endif
		return 1;//Arret brutal du programme
	}

	//BOUCLE

	initialiseThread();

	ordre = "";
	char msg2 [maxChaine];
	char msg3 [maxChaine];

	while(codeRep!=221 && codeRep!=421)
	{
	
        #ifdef DEBUG
            printf("\n>> DEBUG \n");
            printf("Code Reponse recu : %i \n",codeRep);
            printf(">> FIN DEBUG \n\n");
        #endif

		printf("\rftp> ");
		rc = scanf("%s%[^\n]",msg2,msg3);//il y as au maximum 2 arguments
		if (rc == 1)//on entre 1 seul argument
		{
            
            #ifdef DEBUG
                printf("\n>> DEBUG \n");
                printf("Message 2 : %s \n",msg2);
                printf(">> FIN DEBUG \n\n");
            #endif

		rc = transmet(ordre, msg2, reponse);
		}
		else //on entre 2 arguments ou plus
		{

            #ifdef DEBUG
                printf("\n>> DEBUG \n");
                printf("Message 2 : %s \n Message 3 : %s \n",msg2,msg3);
                printf(">> FIN DEBUG \n\n");
            #endif

		rc = transmet(msg2, msg3, reponse);
		}
		codeRep = interpreteReponse (reponse);
	}

	for (rc=0; rc < MAXTHREAD; rc++)
	{
		while (threadPret[rc]!=0)
		{

            #ifdef DEBUG
                printf("\n>> DEBUG \n");
                printf("Thread n°%i : %i \n",rc,threadPret[rc]);
                printf(">> FIN DEBUG \n\n");
            #endif

			system("tput setaf 1");
			printf("Attente de la fin des threads...\n");
			sleep(10);
		}
	}
	#ifdef COULEUR
	system("tput setaf 0");
	#endif
	close(socketControle);
	printf ("\n-- Fin normale de l'execution du client FTP --\n\n");
	return 0; //Termine avec succes
}


int interpreteReponse (char* reponse)//interprete la reponse et renvoie le code reponse correspondant
{
	int i;
	int j=0;
	char rep[4];

    #ifdef DEBUG
        printf("\n<<< Debut de InterpreteReponse >>>\n");
        printf("\n>> DEBUG \n");
        printf("Reponse \n%s",reponse);
        printf(">> FIN DEBUG \n\n");
    #endif

	//on cherche le premier caractere de la derniere ligne
	for (i=0;i<strlen(reponse)-4;i++)
	{
		if (reponse[i] == '\n')
		{
		j = i+1;
		}
	}

    #ifdef DEBUG
        printf("\n>> DEBUG \n");
        printf("Recherche du 1er caractere de la ligne ... [OK]\n");
        printf("i = %i \nj = %i \n",i,j);
        printf(">> FIN DEBUG \n\n");
    #endif

	//on copie le code reponse
	for (i=j;i<j+3;i++)
	{
		rep[i-j] = reponse[i];
	}

    #ifdef DEBUG
        printf("\n>> DEBUG \n");
        printf("Copie du code reponse ... [OK]\n");
        printf("i = %i \nj = %i \n",i,j);
        printf(">> FIN DEBUG \n\n");
    #endif

	rep[3]='\0';
	int codeRep = atoi(rep);

    #ifdef DEBUG
        printf("\n>> DEBUG \n");
        printf("Code Reponse = %i \n",codeRep);
        printf(">> FIN DEBUG \n\n");
    #endif

	if (codeRep == 227)//reponse a une demande de fonctionnement PASSIF
	{
		i=3;//on se place apres le code reponse
		while ( !isdigit(reponse[i]) && i<strlen(reponse))//on cherche le premier chiffre
		{
			++i;
		}

    #ifdef DEBUG
        printf("\n>> DEBUG \n");
        printf("Taille de reponse = %i\n",strlen(reponse));
        printf("i = %i \n",i);
        printf("reponse[i]= %c\n",reponse[i]);
        printf(">> FIN DEBUG \n\n");
    #endif

		if (i==strlen(reponse))
		{
			#ifdef COULEUR
			system("tput setaf 1");
			#endif
			printf("Erreur lors du passage au mode passif\n");
			#ifdef COULEUR
			system("tput setaf 6");
			#endif
			return 500;//erreur
		}
		//else reponse de i est un chiffre
		j = 0;
		char chiffre[3];
		char ip1[4] = "000\0";
		while ( isdigit(reponse[i]) && i<strlen(reponse))
		{
			chiffre[j] = reponse[i];
			++i;
			++j;
		}
		if (reponse[i] != ',' || j==0)
		{
			#ifdef COULEUR
			system("tput setaf 1");
			#endif
			printf("Erreur lors du passage au mode passif\n");
			#ifdef COULEUR
			system("tput setaf 6");
			#endif
			return 500;//erreur
		}
		++i;//else reponse de i est un chiffre
		if (j == 3)
		{
			ip1[2] = chiffre[2];
			ip1[1] = chiffre[1];
			ip1[0] = chiffre[0];
		}
		if (j == 2)
		{
			ip1[2] = chiffre[1];
			ip1[1] = chiffre[0];
		}
		if (j == 1)
		{
			ip1[2] = chiffre[0];
		}

		j = 0;
		char ip2[4] = "000\0";
		while ( isdigit(reponse[i]) && i<strlen(reponse))
		{
			chiffre[j] = reponse[i];
			++i;
			++j;
		}
		if (reponse[i] != ',' || j==0)
		{
			#ifdef COULEUR
			system("tput setaf 1");
			#endif
			printf("Erreur lors du passage au mode passif\n");
			#ifdef COULEUR
			system("tput setaf 6");
			#endif
			return 500;//erreur
		}
		++i;//else reponse de i est un chiffre
		if (j == 3)
		{
			ip2[2] = chiffre[2];
			ip2[1] = chiffre[1];
			ip2[0] = chiffre[0];
		}
		if (j == 2)
		{
			ip2[2] = chiffre[1];
			ip2[1] = chiffre[0];
		}
		if (j == 1)
		{
			ip2[2] = chiffre[0];
		}

		j = 0;
		char ip3[4] = "000\0";
		while ( isdigit(reponse[i]) && i<strlen(reponse))
		{
			chiffre[j] = reponse[i];
			++i;
			++j;
		}
		if (reponse[i] != ',' || j==0)
		{
			#ifdef COULEUR
			system("tput setaf 1");
			#endif
			printf("Erreur lors du passage au mode passif\n");
			#ifdef COULEUR
			system("tput setaf 6");
			#endif
			return 500;//erreur
		}
		++i;//else reponse de i est un chiffre
		if (j == 3)
		{
			ip3[2] = chiffre[2];
			ip3[1] = chiffre[1];
			ip3[0] = chiffre[0];
		}
		if (j == 2)
		{
			ip3[2] = chiffre[1];
			ip3[1] = chiffre[0];
		}
		if (j == 1)
		{
			ip3[2] = chiffre[0];
		}

		j = 0;
		char ip4[4] = "000\0";
		while ( isdigit(reponse[i]) && i<strlen(reponse))
		{
			chiffre[j] = reponse[i];
			++i;
			++j;
		}
		if (reponse[i] != ',' || j==0)
		{
			#ifdef COULEUR
			system("tput setaf 1");
			#endif
			printf("Erreur lors du passage au mode passif\n");
			#ifdef COULEUR
			system("tput setaf 6");
			#endif
			return 500;//erreur
		}
		++i;//else reponse de i est un chiffre
		if (j == 3)
		{
			ip4[2] = chiffre[2];
			ip4[1] = chiffre[1];
			ip4[0] = chiffre[0];
		}
		if (j == 2)
		{
			ip4[2] = chiffre[1];
			ip4[1] = chiffre[0];
		}
		if (j == 1)
		{
			ip4[2] = chiffre[0];
		}

		j = 0;
		char port1[4] = "000\0";
		while ( isdigit(reponse[i]) && i<strlen(reponse))
		{
			chiffre[j] = reponse[i];
			++i;
			++j;
		}
		if (reponse[i] != ',' || j==0)
		{
			#ifdef COULEUR
			system("tput setaf 1");
			#endif
			printf("Erreur lors du passage au mode passif\n");
			#ifdef COULEUR
			system("tput setaf 6");
			#endif
			return 500;//erreur
		}
		++i;//else reponse de i est un chiffre
		if (j == 3)
		{
			port1[2] = chiffre[2];
			port1[1] = chiffre[1];
			port1[0] = chiffre[0];
		}
		if (j == 2)
		{
			port1[2] = chiffre[1];
			port1[1] = chiffre[0];
		}
		if (j == 1)
		{
			port1[2] = chiffre[0];
		}

		j = 0;
		char port2[4] = "000\0";
		while ( isdigit(reponse[i]) && i<strlen(reponse))
		{
			chiffre[j] = reponse[i];
			++i;
			++j;
		}
		if (j==0)
		{
			#ifdef COULEUR
			system("tput setaf 1");
			#endif
			printf("Erreur lors du passage au mode passif\n");
			#ifdef COULEUR
			system("tput setaf 6");
			#endif
			return 500;//erreur
		}
		++i;//else reponse de i est un chiffre
		if (j == 3)
		{
			port2[2] = chiffre[2];
			port2[1] = chiffre[1];
			port2[0] = chiffre[0];
		}
		if (j == 2)
		{
			port2[2] = chiffre[1];
			port2[1] = chiffre[0];
		}
		if (j == 1)
		{
			port2[2] = chiffre[0];
		}

        #ifdef DEBUG
            printf("\n>> DEBUG \n");
            printf("Port2 : %s\n",port2);
            printf(">> FIN DEBUG \n\n");
        #endif

		char ip[16];//taille d'une adresse IP + \0 de fin de chaine
		//construction de l'adresse IP
		ip[0]=ip1[0];
		ip[1]=ip1[1];
		ip[2]=ip1[2];
		ip[3]='.';//caractere '.' ou 0x2E
		ip[4]=ip2[0];
		ip[5]=ip2[1];
		ip[6]=ip2[2];
		ip[7]='.';
		ip[8]=ip3[0];
		ip[9]=ip3[1];
		ip[10]=ip3[2];
		ip[11]='.';
		ip[12]=ip4[0];
		ip[13]=ip4[1];
		ip[14]=ip4[2];
		ip[15]='\0';//fin de l'adresse IP

        #ifdef DEBUG
            printf("\n>> DEBUG \n");
            printf("ip : %s\n",ip);
            printf(">> FIN DEBUG \n\n");
        #endif

		for (j=0;j<15;j++)
		{
			while (ip[j] == '0' && j<15 && ip[j+1]!='.')//une adresse IP valide ne doit pas commencer par zero apres le point
			{
				for (i=j;i<15;i++)
				{
					ip[i]=ip[i+1];
				}
			}
			while (ip[j] != '.' && j<15)
			{
				++j;
			}
		}

		int port = ( (atoi(port1)*256)+ atoi(port2) );//on decode le port

        #ifdef DEBUG
            printf("\n>> DEBUG \n");
            printf("port decode : %i",port);
            printf(">> FIN DEBUG \n\n");
        #endif

		//On ouvre une connection de donnees
		struct hostent *serveur;//declaration du pointeur sur le serveur
		serveur = gethostbyname(ip);//on affecte le serveur
		if (serveur == NULL)//si serveur non valide
		{
			system("tput setaf 1");
			printf ("L'adresse de destination du socket de données n'est pas valide\n");
			system("tput setaf 6");
			return 500;//Erreur standard
		}

		struct sockaddr_in servAddr;//declaration du socket serveur
		servAddr.sin_family = serveur->h_addrtype;//configuration du socket
		memcpy((char *) &servAddr.sin_addr.s_addr, serveur->h_addr_list[0], serveur->h_length);//On associe le socket serveur au serveur
		servAddr.sin_port = htons(port);//on parametre le socket pour le port voulu

        #ifdef DEBUG
        printf("\n>> DEBUG \n");
        printf("Socket serveur :\n");
        printf("Adresse : %i \nPort : %i \n", ntohl(servAddr.sin_addr.s_addr), ntohs(servAddr.sin_port));
        printf(">> FIN DEBUG \n\n");
        #endif

		int dataSocket = socket(AF_INET, SOCK_STREAM, 0);//ouverture du socket

		if(dataSocket<0) //si l'ouverture echoue
		{
			#ifdef COULEUR
			system("tput setaf 1");
			#endif
			printf("Impossible d'ouvrir le socket\n");
			#ifdef COULEUR
			system("tput setaf 6");
			#endif
			return 500;//Erreur standard
		}

		/*de meme on cree une adresse local*/
		struct sockaddr_in localAddr;
		localAddr.sin_family = AF_INET;
		localAddr.sin_addr.s_addr = htonl(INADDR_ANY);
		localAddr.sin_port = htons(0);//le port utilisé sur le client peut etre quelconque

        #ifdef DEBUG
        printf("\n>> DEBUG \n");
        printf("Socket client :\n");
        printf("Adresse : %i \nPort : %i \n", ntohl(localAddr.sin_addr.s_addr), ntohs(localAddr.sin_port));
        printf(">> FIN DEBUG \n\n");
        #endif

		//connexion au serveur
		if(connect(dataSocket,(struct sockaddr *) &servAddr,sizeof(servAddr)) < 0) //si la connexion echoue
		{
			#ifdef COULEUR
			system("tput setaf 1");
			#endif
			printf("Connexion impossible\n");
			#ifdef COULEUR
			system("tput setaf 6");
			#endif
			return 500;//Erreur standard
		}
		//on enregistre le socket de donnees
		socketTab[trouveThreadLibre()] = dataSocket;
		threadPret[trouveThreadLibre()] = 1;
	}
	
	if (codeRep == 150)//ouverture du canal de donnees
	{
		pthread_create (&threadID[trouveThreadPret()], NULL , thread , (void*) trouveThreadPret());
	}
	
	if (codeRep == 226)//pareil que 150 mais deja aquite
	{
		Ack[trouveThreadPret()]=1;//reponse deja aquite
		pthread_create (&threadID[trouveThreadPret()], NULL , thread , (void*) trouveThreadPret());
	}

    #ifdef DEBUG
        printf("\n>> DEBUG \n");
        printf("Code reponse envoyer : %i \n",codeRep);
        printf(">> FIN DEBUG \n\n");
        printf("<<< Fin de InterpreteReponse >>>\n\n");
    #endif

	return codeRep;
}


void* thread (void* parametre)//processus associe a un thread
{
	int numero = ((int)parametre);//on recupere le numero du thread

    #ifdef DEBUG
        printf("<<< Debut de thread >>>\n\n");
        printf("\n>> DEBUG \n");
        printf("Numero du thread : %i \n",numero);
        printf(">> FIN DEBUG \n\n");
    #endif

	threadPret[numero] = 2;//on declare le thread en actif
	int dataSocket = socketTab[0];//on restaure la socket

    #ifdef DEBUG
        printf("\n>> DEBUG \n");
        printf("dataSocket : %i \n",dataSocket);
        printf(">> FIN DEBUG \n\n");
    #endif

	char reponse[maxMsg];
	memset(reponse,0x0,maxMsg);
	int i;
	if (action[numero] == 0)//on affiche le resultat
	{
		i = recv(dataSocket, reponse, maxMsg-1, 0);
		if (i<0)
		{
			#ifdef COULEUR
			system("tput setaf 1");
			#endif
			printf("Impossible de recevoir des donnees\n");
			close (dataSocket);
			threadPret[numero] = 0;//on declare le thread en libre
			#ifdef COULEUR
			system("tput setaf 6");
			#endif
			return (void*)1;
		}
		#ifdef COULEUR
		system("tput setaf 4");
		#endif
		printf ("\r");
		printf (reponse);
		while (reponse[i-1]!= '\n')
		{
			memset(reponse,0x0,maxMsg);
			i = recv(dataSocket, reponse, maxMsg-1, 0);
			if (i<0)
			{
				#ifdef COULEUR
				system("tput setaf 1");
				#endif
				printf("Impossible de recevoir des donnees\n");
				close (dataSocket);
				threadPret[numero] = 0;//on declare le thread en libre
				#ifdef COULEUR
				system("tput setaf 6");
				#endif
				return (void*)1;
			}
			printf (reponse);
		}
	}

	if (action[numero] == 1)//on enregistre le resultat dans un fichier
	{
		char fich[maxChaine];
		strcpy(fich,nomFichier);//BUG: il faut mettre une exclusion mutuelle
		fich[strlen(fich)-2]='\0';//on suprime le \n du nom du fichier

                #ifdef DEBUG
                        printf("\n\n>> DEBUG \n");
                        printf("fich : %s \n",fich);
                        printf(">> FIN DEBUG \n\n");
                #endif

		int taille = tailleFichier;

                #ifdef DEBUG
                        printf("\n\n>> DEBUG \n");
                        printf("taille avant comptage: %i \n",taille);
                        printf(">> FIN DEBUG \n\n");
                #endif

		i = 0;
		while (taille > maxMsg-1)//on compte le nombre d'iteration necessaire
		{
			taille = taille - (maxMsg-1);
			++i;
		}

                #ifdef DEBUG
                        printf("\n\n>> DEBUG \n");
                        printf("taille après comptage : %i \n",taille);
                        printf(">> FIN DEBUG \n\n");
                #endif

		int fichier;//on declare un descripteur de fichier
		fichier=open(fich, O_WRONLY | O_CREAT| O_TRUNC, S_IRWXU);
		if (fichier== -1)//si la creation echoue
		{
			#ifdef COULEUR
			system("tput setaf 1");
			#endif
			printf("La création du fichier à echoué.\n");
			close (dataSocket);
			threadPret[numero] = 0;//on declare le thread en libre
			#ifdef COULEUR
			system("tput setaf 6");
			#endif
			return (void*)2;
		}
		int j;
		int avancement=0;
		int k;
		#ifdef COULEUR
		system("tput setaf 2");
		#endif
		for (j=0;j<i;j++)
		{
			if (Ack[numero] == 0)
			{
				avancement=(int)((((float)j+1.0)/(float)i)*100.0);
				printf("\rReception:\t%i %%\t[",avancement);
				for (k=0;k<=100;k=k+2)
				{
					if (avancement >= k)
					{
						printf("=");
					}
					else if ((avancement < k) && (avancement >= k-2))
					{
						printf(">");
					}
					else
					{
						printf(" ");
					}
				}
				printf("]\r");
				fflush(stdout);
			}
			if (recv(dataSocket, reponse, maxMsg-1, MSG_WAITALL)<0)
			{
				#ifdef COULEUR
				system("tput setaf 1");
				#endif
				printf("Impossible de recevoir des donnees\n");
				close (dataSocket);
				threadPret[numero] = 0;//on declare le thread en libre
				#ifdef COULEUR
				system("tput setaf 6");
				#endif
				return (void*)1;
			}
			write(fichier,reponse,maxMsg-1);
		}

                #ifdef DEBUG
                        printf("\n\n>> DEBUG \n");
                        printf("avancement : %i \n",avancement);
                        printf(">> FIN DEBUG \n\n");
                #endif

		if (taille != 0)
		{
			sleep(1);//on attend que le tampon soit rempli
			memset(reponse,0x0,maxMsg);
			i = recv(dataSocket, reponse, maxMsg-1, 0);
			if (i<0)
			{
				#ifdef COULEUR
				system("tput setaf 1");
				#endif
				printf("Impossible de recevoir des donnees\n");
				close (dataSocket);
				threadPret[numero] = 0;//on declare le thread en libre
				#ifdef COULEUR
				system("tput setaf 6");
				#endif
				return (void*)1;
			}
			write(fichier,reponse,i);
		}
		printf("\n");
		close (fichier);//on ferme le fichier
	}

	if (action[numero] == 2)//on envoie un fichier
	{
		char fich[maxChaine];
		strcpy(fich,nomFichier);//BUG: il faut mettre une exclusion mutuelle
		fich[strlen(fich)-2]='\0';//on suprime le \n du nom du fichier

                #ifdef DEBUG
                        printf("\n\n>> DEBUG \n");
                        printf("fich : %s \n",fich);
                        printf(">> FIN DEBUG \n\n");
                #endif

		int taille = 0;
		int fichier;//on declare un descripteur de fichier
		fichier=open(fich, O_RDONLY );
		if (fichier== -1)//si l'ouverture echoue
		{
			#ifdef COULEUR
			system("tput setaf 1");
			#endif
			printf("Impossible d'ouvrir le fichier.\n");
			close (dataSocket);
			threadPret[numero] = 0;//on declare le thread en libre
			#ifdef COULEUR
			system("tput setaf 6");
			#endif
			return (void*)2;
		}
		char c;
		while (read (fichier, &c,sizeof(char)) > 0 )//on compte la taille du fichier
		{
			++taille;
		}

                #ifdef DEBUG
                        printf("\n\n>> DEBUG \n");
                        printf("taille : %i \n",taille);
                        printf(">> FIN DEBUG \n\n");
                #endif

		i = 1;
		while (taille > maxMsg-1)//on compte le nombre d'iteration necessaire
		{
			taille = taille - (maxMsg-1);
			++i;
		}
		close (fichier);//on ferme le fichier

                #ifdef DEBUG
                        printf("\n\n>> DEBUG \n");
                        printf("nombre d'iterations : %i \n",taille);
                        printf(">> FIN DEBUG \n\n");
                #endif

		fichier=open(fich, O_RDONLY );//on reouvre le fichier pour etre au debut
		if (fichier== -1)//si l'ouverture echoue
		{
			#ifdef COULEUR
			system("tput setaf 1");
			#endif
			printf("Impossible d'ouvrir le fichier.\n");
			close (dataSocket);
			threadPret[numero] = 0;//on declare le thread en libre
			#ifdef COULEUR
			system("tput setaf 6");
			#endif
			return (void*)2;
		}
		int j=0;
		int avancement=0;
		int k;
		int l;//nb de caracteres qui sont copies dans le tampon d'envoie
		#ifdef COULEUR
		system("tput setaf 2");
		#endif

		for (j=0;j<i;j++)
		{
			avancement=(int)((((float)j+1.0)/(float)i)*100.0);
			printf("\rEnvoi:\t\t%i %%\t[",avancement);
			for (k=0;k<=100;k=k+2)
			{
				if (avancement >= k)
				{
					printf("=");
				}
				else if ((avancement < k) && (avancement >= k-2))
				{
					printf(">");
				}
				else
				{
					printf(" ");
				}
			}
			printf("]\r");
			fflush(stdout);
			l=0;
			memset(reponse,0x0,maxMsg);
			while (read (fichier, &c,sizeof(char)) > 0 && l<maxMsg-2)//on compte la taille du fichier
			{
				reponse[l]=c; //ici on utilise la variable reponse pour stocker le message a envoyer
				++l;
			}
			if (l>=maxMsg-2)//on copie le dernier caractere
			{
				reponse[l]=c; //ici on utilise la variable reponse pour stocker le message a envoyer
				++l;
			}
			if (send(dataSocket, reponse, l, 0) <0) //Envoie de donnee
			{
				#ifdef COULEUR
				system("tput setaf 1");
				#endif
				printf("Impossible d'envoyer le fichier.\n");
				close (dataSocket);
				threadPret[numero] = 0;//on declare le thread en libre
				#ifdef COULEUR
				system("tput setaf 6");
				#endif
				return (void*)1;
			}
		}

                #ifdef DEBUG
                        printf("\n\n>> DEBUG \n");
                        printf("avancement : %i \n",avancement);
                        printf(">> FIN DEBUG \n\n");
                #endif

		printf("\n");
		close (fichier);//on ferme le fichier
	}

	close (dataSocket);//certains serveurs attendent la fin de connection pour acquiter
	if (Ack[numero] == 0)//si l'aquitement de fin de tranfert n'est pas effectue
	{
		memset(reponse,0x0,maxMsg);
		i = recv(socketControle, reponse, maxMsg-1, 0);
		if (i<0)
		{
			#ifdef COULEUR
			system("tput setaf 1");
			#endif
			printf("Impossible de recevoir l'aquitement\n");
			close (dataSocket);
			threadPret[numero] = 0;//on declare le thread en libre
			#ifdef COULEUR
			system("tput setaf 6");
			#endif
			return (void*)3;
		}
		#ifdef COULEUR
		system("tput setaf 4");
		#endif
		printf("\r%s",reponse);//on affiche le datagramme acquitement
		#ifdef COULEUR
		system("tput setaf 6");
		#endif
		printf("\rftp> ");
		fflush(stdout);
	}
	else//on reinitialise le thread
	{
		Ack[numero]=0;
	}
	threadPret[numero] = 0;//on declare le thread en libre
	#ifdef COULEUR
	system("tput setaf 6");
	#endif
	printf("\rftp> ");
	fflush(stdout);

        #ifdef DEBUG
           printf("<<< Fin de thread >>>\n\n");
        #endif

	return (void*)0;
}


char* interpreteCommande (char* commande, int taille)
{

        #ifdef DEBUG
             printf("\n\n<<< Debut de interpreteCommande >>>\n\n");
        #endif

	int j = 4;
	if (taille < 4)//une commande a au max 4 caracteres
	{
		j = taille;
	}

        #ifdef DEBUG
             printf("\n\n>> DEBUG \n");
             printf("Taille de la commande : %i \n",taille);
             printf(">> FIN DEBUG \n\n");
        #endif

	char com[5]="\0\0\0\0\0";
	int i;
		
	for (i=0;i<j;i++)
	{
		com[i] = commande[i];
	}

        #ifdef DEBUG
              printf("\n\n>> DEBUG \n");
              printf("commande : %s \n",com);
              printf(">> FIN DEBUG \n\n");
        #endif

	if ( strcasecmp(com,"LIST") == 0 )//si la commande est list
	{
		action[trouveThreadPret()] = 0;//on indique au prochain thread d'afficher son resultat
	}
	
	if ( strcasecmp(com,"RETR") == 0 )//si la commande est RETR
	{
		action[trouveThreadPret()] = 1;//on indique au prochain thread d'enregistrer son resultat dans un fichier
		memset(nomFichier,0x0,maxChaine);//on initialise le nomFichier avec des zeros
		++j;//j pointe sur le premier caractere de l'argument
		for (i=0;i<taille;i++)//on enregistre le nom du fichier
		{
			nomFichier[i]=commande[i+j];
		}
		tailleFichier = getsize (nomFichier);

                #ifdef DEBUG
                printf("\n\n>> DEBUG \n");
                printf("Nom fichier : %s \ntaille fichier : %d \n",nomFichier, tailleFichier);
                printf(">> FIN DEBUG \n\n");
                #endif

	}

	if ( strcasecmp(com,"GET ") == 0 )//si la commande est GET
	{
		initialiseTransfert ();
		action[trouveThreadPret()] = 1;//on indique au prochain thread d'enregistrer son resultat dans un fichier
		memset(nomFichier,0x0,maxChaine);//on initialise le nomFichier avec des zeros
		for (i=0;i<taille;i++)//on enregistre le nom du fichier
		{
			nomFichier[i]=commande[i+j];
		}
		getPWD();
		tailleFichier = getsize (nomFichier);
                
                #ifdef DEBUG
                        printf("\n\n>> DEBUG \n");
                        printf("Nom fichier : %s \ntaille fichier : %d \n",nomFichier, tailleFichier);
                        printf(">> FIN DEBUG \n\n");
                #endif

		// on ajoute le PWD a la commande

                #ifdef DEBUG
                        printf("\n\n>> DEBUG \n");
                        printf("Debut PWD\n");
                        printf(">> FIN DEBUG \n\n");
                #endif

		int k;
		for (i=0;i<strlen(dossierCourant);i++)
		{
			k = strlen(commande);
			for (j=k;j>3+i;j--)
			{
				commande[j+1]=commande[j];
			}
			commande[4+i]=dossierCourant[i];
		}
		// on ajoute un décalage
		k = strlen(commande);
		for (j=k;j>3;j--)
		{
			commande[j+1]=commande[j];
		}

                #ifdef DEBUG
                        printf("\n\n>> DEBUG \n");
                        printf("Commande avant remplacement: %s\n",commande);
                        printf(">> FIN DEBUG \n\n");
                #endif

		//on remplace la commande par RETR
		commande[0]='R';
		commande[1]='E';
		commande[2]='T';
		commande[3]='R';
		commande[4]=' ';

                #ifdef DEBUG
                        printf("\n\n>> DEBUG \n");
                        printf("Commande après remplacement: %s\n",commande);
                        printf(">> FIN DEBUG \n\n");
                #endif

	}

	if ( strcasecmp(com,"HELP") == 0 )//si la commande est HELP
	{
		#ifdef COULEUR
		system("tput setaf 2");//ecrire en vert
		#endif
		printf("-- Les commandes propres au client sont:\n");
		printf("LS \t\t(automatise le listage des fichiers du serveur)\n");
		printf("GET fichier\t(automatise la réception du fichier)\n");
		printf("PUT fichier\t(automatise l'envoie du fichier)\n");
		printf("-- Toutes autres commandes seront envoyés au serveur\n");
		#ifdef COULEUR
		system("tput setaf 1");//ecrire en bleu
		#endif
	}


	if ( strcasecmp(com,"ls") == 0 )//si la commande est ls
	{
		//passage en PASV
		char reponse[maxMsg];//message a recevoir
		transmet("PASV", "", reponse);
		if (interpreteReponse (reponse) != 227)
		{
			#ifdef COULEUR
			system("tput setaf 1");
			#endif
			printf("Erreur: reponse inatendue\n");
			#ifdef COULEUR
			system("tput setaf 0");//ecrire en noir
			#endif
		}
		action[trouveThreadPret()] = 0;//on indique au prochain thread d'afficher son resultat
		//on remplace la commande par list -la
		commande[0]='L';
		commande[1]='I';
		commande[2]='S';
		commande[3]='T';
		commande[4]=' ';
		commande[5]='-';
		commande[6]='a';
		commande[7]='l';
		commande[8]='\n';
		commande[9]='\0';
	}

	if ( strcasecmp(com,"PUT ") == 0 )//si la commande est PUT
	{
		initialiseTransfert ();
		action[trouveThreadPret()] = 2;//on indique au prochain thread de lire dans un fichier
		memset(nomFichier,0x0,maxChaine);//on initialise le nomFichier avec des zeros
		for (i=0;i<taille;i++)//on enregistre le nom du fichier
		{
			nomFichier[i]=commande[i+j];
		}
		getPWD();
                
		// on ajoute le PWD a la commande
		int k;
		for (i=0;i<strlen(dossierCourant);i++)
		{
			k = strlen(commande);
			for (j=k;j>3+i;j--)
			{
				commande[j+1]=commande[j];
			}
			commande[4+i]=dossierCourant[i];
		}
		// on ajoute un décalage
		k = strlen(commande);
		for (j=k;j>3;j--)
		{
			commande[j+1]=commande[j];
		}

		//on remplace la commande par RETR
		commande[0]='S';
		commande[1]='T';
		commande[2]='O';
		commande[3]='R';
		commande[4]=' ';

                #ifdef DEBUG
                        printf("\n\n>> DEBUG \n");
                        printf("Commande après remplacement: %s\n",commande);
                        printf(">> FIN DEBUG \n\n");
                #endif

	}

        #ifdef DEBUG
            printf("\n\n<<< Fin de interpreteCommande >>>\n\n");
        #endif

	return commande;
}


int transmet (char* ordre, char* messageAvecEspace, char* reponse)
{

        #ifdef DEBUG
            printf("\n\n<<< Debut de transmet >>>\n\n");
        #endif

	int i=0;
	int j=0;

        #ifdef DEBUG
            printf("\n\n>> DEBUG \n");
            printf("Message avant suppresion des espaces: %s\n",messageAvecEspace);
            printf(">> FIN DEBUG \n\n");
        #endif

	//Supprime les espaces en debut du message
	char message[strlen(messageAvecEspace)+1];//+1 pour le '\0'
	while (messageAvecEspace[i] == 0x20)
	{
		++i;
	}
		
	for (; i< strlen(messageAvecEspace) ; i++ )
	{
		message[j]=messageAvecEspace[i];
		++j;//j pointe sur le prochain caractere
	}
	message[j]='\0';
	//Suppression des espaces terminee

        #ifdef DEBUG
            printf("\n\n>> DEBUG \n");
            printf("Message après suppresion des espaces: %s\n",message);
            printf(">> FIN DEBUG \n\n");
        #endif

	int tailleOrdre = strlen(ordre);
	if (tailleOrdre > 1)//si il y as un ordre
	{
		++tailleOrdre;//on compte la taille de l'espace entre ordre et message
	}
	else
	{
		tailleOrdre = 0;
	}
	
	char snd_msg[(strlen(message)+tailleOrdre+3)];//datagramme a envoyer
	//copie manuelle des caractéres dans le datagramme
	if (tailleOrdre != 0)//si il y as un ordre
	{
		for (i=0;i<(tailleOrdre-1);i++)
		{
			snd_msg[i] = ordre[i];
		}
		snd_msg[tailleOrdre-1] = 0x20;//separateur ordre - message
	}
	
	//copie manuelle des caractéres dans le datagramme
	j=tailleOrdre;
	for (i=0;i<(strlen(message));i++)
	{
		snd_msg[j] = message[i];
		++j;
	}
	snd_msg[strlen(message)+tailleOrdre] = 0x0d;//indique la fin d'instruction FTP \r
	snd_msg[strlen(message)+tailleOrdre+1] = 0x0a;//caractere: retour chariot \n
	snd_msg[strlen(message)+tailleOrdre+2] = '\0';//caractere de fin de chaine

        #ifdef DEBUG
                printf("\n\n>> DEBUG \n");
                printf("snd_msg avant interpretation : %s\n",snd_msg);
                printf(">> FIN DEBUG \n\n");
        #endif

	strcpy (snd_msg,interpreteCommande(snd_msg,strlen(snd_msg)-2));

        #ifdef DEBUG
                printf("\n\n>> DEBUG \n");
                printf("snd_msg après interpretation : %s\n",snd_msg);
                printf(">> FIN DEBUG \n\n");
        #endif

	i = send(socketControle, snd_msg, strlen(snd_msg), 0);//Envoie de donnee
	if(i<0) //echec de l'envoie
	{
		printf("Echec lors de l'envoie de donnees\n");
		return 1;//Arret brutal du programme
	}
	sleep(1);//temporisation de une seconde pour que l'integralite de la reponse soit stoke dans le tampon de reception, celle ci etant parfois fragmente en plusieurs paquets

	memset(reponse,0x0,maxMsg);//on prepare la zone memoire en la remplissant par le caractere '\0', utile car le pacquet recu ne comptiend pas ce caractere de fin de chaine
	i = recv(socketControle, reponse, maxMsg-1, 0);
	if (i<0)
	{
		printf("Impossible de recevoir des donnees\n");
		return 1;//Arret brutal du programme
	}

	#ifdef COULEUR
	system("tput setaf 4");
	if (reponse[0] == '5')//si c'est un message d'erreur
	{
	system("tput setaf 1");
	}
	#endif
	printf(reponse);//on affiche le datagramme reponse
	#ifdef COULEUR
	system("tput setaf 6");
	#endif

        #ifdef DEBUG
            printf("\n\n<<< Fin de transmet >>>\n\n");
        #endif

	return 0;//exit_success
}


int getsize (char* fich)//retourne la taille du fichier en octets
{

        #ifdef DEBUG
            printf("\n\n<<< Debut de getsize >>>\n\n");
        #endif

	int i=0;
	int j=0;
	char reponse[maxMsg];
	char ordre[5] = "SIZE\0";
	int tailleOrdre = strlen(ordre);
	char snd_msg[strlen(dossierCourant)+(strlen(fich)+tailleOrdre+1)];//datagramme a envoyer
	//copie manuelle des caractéres dans le datagramme
	if (tailleOrdre != 0)//si il y as un ordre
	{
		for (i=0;i<(tailleOrdre);i++)
		{
			snd_msg[i] = ordre[i];
		}
		snd_msg[tailleOrdre] = 0x20;//separateur ordre - message
	}

        #ifdef DEBUG
            printf("\n\n>> DEBUG \n");
            printf("snd_msg avant copie des caracteres : %s\n",snd_msg);
            printf(">> FIN DEBUG \n\n");
        #endif

	//copie manuelle des caractéres dans le datagramme
	j=tailleOrdre+1;
	for (i=0;i<(strlen(dossierCourant));i++)
	{
		snd_msg[j] = dossierCourant[i];
		++j;
	}
	for (i=0;i<(strlen(fich));i++)
	{
		snd_msg[j] = fich[i];
		++j;
	}
	snd_msg[strlen(dossierCourant)+strlen(fich)+tailleOrdre+1] = '\0';//caractere de fin de chaine
	
        #ifdef DEBUG
            printf("\n\n>> DEBUG \n");
            printf("snd_msg après copie des caracteres : %s\n",snd_msg);
            printf(">> FIN DEBUG \n\n");
        #endif

	i = send(socketControle, snd_msg, strlen(snd_msg), 0);//Envoie de donnee
	if(i<0) //echec de l'envoie
	{
		system("tput setaf 1");
		printf("Echec lors de l'envoie de donnees\n");
		system("tput setaf 6");
		return 1;//Arret brutal du programme
	}
	sleep(1);//temporisation de une seconde pour que l'integralite de la reponse soit stoke dans le tampon de reception, celle ci etant parfois fragmente en plusieurs paquets

	memset(reponse,0x0,maxMsg);//on prepare la zone memoire en la remplissant par le caractere '\0', utile car le pacquet recu ne comptiend pas ce caractere de fin de chaine
	i = recv(socketControle, reponse, maxMsg-1, 0);
	if (i<0)
	{
		#ifdef COULEUR
		system("tput setaf 1");
		#endif
		printf("Impossible de recevoir des donnees\n");
		#ifdef COULEUR
		system("tput setaf 6");
		#endif
		return 1;//Arret brutal du programme
	}
	if (interpreteReponse(reponse) != 213)
	{
		#ifdef COULEUR
		system("tput setaf 1");
		#endif
		printf("Impossible d'aquérir la taille du fichier\n");
		#ifdef COULEUR
		system("tput setaf 6");
		#endif
		return 1;//Arret brutal du programme
	}
	char taille[strlen(reponse)];
	i = 4;
	j = 0;
	while (i<strlen(reponse))
	{
		if (isdigit(reponse[i]))
		{
			taille[j]=reponse[i];
			++j;
		}
		++i;
	}
	taille[j]='\0';

        #ifdef DEBUG
            printf("\n\n>> DEBUG \n");
            printf("taille : %s\n",taille);
            printf(">> FIN DEBUG \n\n");
            printf("\n\n<<< Fin de getsize >>>\n\n");
        #endif

	return atoi(taille);
}


void getPWD ()
{

        #ifdef DEBUG
            printf("\n\n<<< Debut de getPWD >>>\n\n");
        #endif

	int i=0;
	char reponse[maxMsg];
	char snd_msg[6]= "PWD\r\n\0";//datagramme a envoyer
	i = send(socketControle, snd_msg, strlen(snd_msg), 0);//Envoie de donnee
	if(i<0) //echec de l'envoie
	{
		#ifdef COULEUR
		system("tput setaf 1");
		#endif
		printf("Echec lors de l'envoie de donnees\n");
		#ifdef COULEUR
		system("tput setaf 6");
		#endif
		exit (1);//Arret brutal du programme
	}
	sleep(1);//temporisation de une seconde pour que l'integralite de la reponse soit stoke dans le tampon de reception, celle ci etant parfois fragmente en plusieurs paquets

	memset(reponse,0x0,maxMsg);//on prepare la zone memoire en la remplissant par le caractere '\0', utile car le pacquet recu ne comptiend pas ce caractere de fin de chaine
	i = recv(socketControle, reponse, maxMsg-1, 0);
	if (i<0)
	{
		#ifdef COULEUR
		system("tput setaf 1");
		#endif
		printf("Impossible de recevoir des donnees\n");
		#ifdef COULEUR
		system("tput setaf 6");
		#endif
		exit (1);//Arret brutal du programme
	}

        #ifdef DEBUG
            printf("\n\n>> DEBUG \n");
            printf("Code reponse : %i\n",interpreteReponse(reponse));
            printf(">> FIN DEBUG \n\n");
        #endif

	if (interpreteReponse(reponse) != 257)
	{
		#ifdef COULEUR
		system("tput setaf 1");
		#endif
		printf("Impossible d'aquérir l'emplacement courant\n");
		#ifdef COULEUR
		system("tput setaf 6");
		#endif
		exit (1);//Arret brutal du programme
	}
	i = 4;
	int j = 0;
	while (i<strlen(reponse) && reponse[i]!='"')//on cherche le début de la chaine
	{
	++i;
	}
	++i;
	while (i<strlen(reponse) && reponse[i]!='"' && j<maxChaine)//tant que non fin de chaine
	{
		dossierCourant[j]=reponse[i];
		++j;
		++i;
	}

        #ifdef DEBUG
            printf("\n\n>> DEBUG \n");
            printf("Dossier Courant : %s\n",dossierCourant);
            printf(">> FIN DEBUG \n\n");
        #endif

	if (dossierCourant[j-1] != '/')//un dossier doit se terminer par un slash
	{
		dossierCourant[j] = '/';
		++j;
	}
	dossierCourant[j]='\0';

        #ifdef DEBUG
            printf("\n\n<<< Fin de getPWD >>>\n\n");
        #endif

}


void initialiseTransfert ()//envoie TYPE I puis PASV
{

        #ifdef DEBUG
           printf("\n\n<<< Debut de initialiseTransfert >>>\n\n");
        #endif

	int i=0;
	char reponse[maxMsg];
	char snd_msg[9]="TYPE I\r\n\0";//datagramme a envoyer
	i = send(socketControle, snd_msg, strlen(snd_msg), 0);//Envoie de donnee
	if(i<0) //echec de l'envoie
	{
		#ifdef COULEUR
		system("tput setaf 1");
		#endif
		printf("Echec lors de l'envoie de donnees\n");
		#ifdef COULEUR
		system("tput setaf 6");
		#endif
		exit (1);//Arret brutal du programme
	}
	sleep(1);//temporisation de une seconde pour que l'integralite de la reponse soit stoke dans le tampon de reception, celle ci etant parfois fragmente en plusieurs paquets

	memset(reponse,0x0,maxMsg);//on prepare la zone memoire en la remplissant par le caractere '\0', utile car le pacquet recu ne comptiend pas ce caractere de fin de chaine
	i = recv(socketControle, reponse, maxMsg-1, 0);
	if (i<0)
	{
		#ifdef COULEUR
		system("tput setaf 1");
		#endif
		printf("Impossible de recevoir des donnees\n");
		#ifdef COULEUR
		system("tput setaf 6");
		#endif
		exit (1);//Arret brutal du programme
	}
	#ifdef COULEUR
	system("tput setaf 4");
	#endif
	printf(reponse);//on affiche le datagramme reponse

        #ifdef DEBUG
           printf("\n\n>> DEBUG \n");
           printf("Code reponse : %i\n",interpreteReponse(reponse));
           printf(">> FIN DEBUG \n\n");
        #endif

	if (interpreteReponse(reponse) != 200)
	{
		#ifdef COULEUR
		system("tput setaf 1");
		#endif
		printf("Impossible de passer en mode binaire\n");
		#ifdef COULEUR
		system("tput setaf 6");
		#endif
		exit (1);//Arret brutal du programme
	}

	char snd_msg2[7]="PASV\r\n\0";//datagramme a envoyer
	i = send(socketControle, snd_msg2, strlen(snd_msg2), 0);//Envoie de donnee
	if(i<0) //echec de l'envoie
	{
		#ifdef COULEUR
		system("tput setaf 1");
		#endif
		printf("Echec lors de l'envoie de donnees\n");
		#ifdef COULEUR
		system("tput setaf 6");
		#endif
		exit (1);//Arret brutal du programme
	}
	sleep(1);//temporisation de une seconde pour que l'integralite de la reponse soit stoke dans le tampon de reception, celle ci etant parfois fragmente en plusieurs paquets

	memset(reponse,0x0,maxMsg);//on prepare la zone memoire en la remplissant par le caractere '\0', utile car le pacquet recu ne comptiend pas ce caractere de fin de chaine
	i = recv(socketControle, reponse, maxMsg-1, 0);
	if (i<0)
	{
		#ifdef COULEUR
		system("tput setaf 1");
		#endif
		printf("Impossible de recevoir des donnees\n");
		#ifdef COULEUR
		system("tput setaf 6");
		#endif
		exit (1);//Arret brutal du programme
	}
	printf(reponse);//on affiche le datagramme reponse
	if (interpreteReponse(reponse) != 227)
	{
		#ifdef COULEUR
		system("tput setaf 1");
		#endif
		printf("Impossible de passer en mode passif\n");
		#ifdef COULEUR
		system("tput setaf 6");
		#endif
		exit (1);//Arret brutal du programme
	}

        #ifdef DEBUG
           printf("\n\n<<< Fin de initialiseTransfert >>>\n\n");
        #endif

}


void initialiseThread ()//met a zero les tableaux de threads
{

        #ifdef DEBUG
           printf("\n\n<<< Debut de initialiseThread >>>\n\n");
        #endif

	int i;
	for (i=0;i<MAXTHREAD;i++)
	{
		action[i]=0;//a effectuer par le thread: 0=afficher 1=ecrire en fichier 2=envoie de fichier
		threadPret[i]=0;//0=libre 1=pret 2=actif
		Ack[i]=0;//rien n'est aquite
	}

        #ifdef DEBUG
           printf("\n\n<<< Fin de initialiseThread >>>\n\n");
        #endif

}


int trouveThreadLibre ()//renvoie le chiffre du premier thread libre
{

        #ifdef DEBUG
           printf("\n\n<<< Debut de trouveThreadLibre >>>\n\n");
        #endif

	int i=0;
	while (threadPret[i]!=0 && i < MAXTHREAD)
	{
		++i;
	}

        #ifdef DEBUG
           printf("\n\n>> DEBUG \n");
           printf("Thread libre : n°%i\n",i);
           printf(">> FIN DEBUG \n\n");
        #endif

	if (i == MAXTHREAD)//si plus de thread libre
	{	//on attend qu'il y en ais un de libre...
		#ifdef COULEUR
		system("tput setaf 1");
		#endif
		printf("Il n'y as plus de threads de disponible, nous attendons qu'un thread se libére...\n");
		sleep(10);
		i=trouveThreadLibre();//appel recursif
		#ifdef COULEUR
		system("tput setaf 6");
		#endif
	}

        #ifdef DEBUG
           printf("\n\n<<< Fin de trouveThreadLibre >>>\n\n");
        #endif

	return i;
}


int trouveThreadPret ()//renvoie le chiffre du premier thread pret
{

        #ifdef DEBUG
           printf("\n\n<<< Debut de trouveThreadPret >>>\n\n");
        #endif

	int i=0;
	while (threadPret[i]!=1 && i < MAXTHREAD)
	{
		++i;
	}

        #ifdef DEBUG
           printf("\n\n>> DEBUG \n");
           printf("Thread pret : n°%i\n",i);
           printf(">> FIN DEBUG \n\n");
        #endif

	if (i == MAXTHREAD)//si plus de thread pret
	{
		#ifdef COULEUR
		system("tput setaf 1");
		#endif
		printf("Il n'y as aucun thread de pret. Utilisez PASV pour en preparer un.\n");
		#ifdef COULEUR
		system("tput setaf 6");
		#endif
	}

        #ifdef DEBUG
          printf("\n\n<<< Fin de trouveThreadPret >>>\n\n");
        #endif

	return i;
}

