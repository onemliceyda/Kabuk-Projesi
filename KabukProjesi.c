#include <stdio.h> //input,output kütüphanesi

#include <stdlib.h> //dinamik bellek yönetimi için

#include <stdbool.h> //true ,false ...

#include <string.h> //strtok() ~ strcmp() vs. string işlemleri için

#include <sys/wait.h> //waitpid(), wait()

#include <unistd.h> //fork(), execvp(), pid_t strcut'ı için

#include <limits.h> //max, min için

int QUIT = 0; //exit komutunu çalıştırmak için
bool kontrol = false; //512 karakterin kontrolü için tanımlandı
int i = 0; //gezici eleman olarak tanımlandı
int komutSayisi = 0; //komut sayısını tutması için

//kullanıcının kullanıcı-dizin bilgisi (prompt)
void komutIstemi() {
    i = 0;
    //kullanacağımız özelliklerin char dizi ~ string tanımları
    char anaBilgisayarAdi[HOST_NAME_MAX];
	char kullaniciAdi[LOGIN_NAME_MAX];
	char dosyaKonum[1024];

	// getcwd dosyanın anlık konumunu getirir
	if (getcwd(dosyaKonum, sizeof(dosyaKonum)) != NULL) {

		//anaBilgisayarAdi
		gethostname(anaBilgisayarAdi, HOST_NAME_MAX);
		//kullaniciAdi
		getlogin_r(kullaniciAdi, LOGIN_NAME_MAX);

		//bilgileri ekrana basma
		fprintf(stderr, "\033[0m\033[1;93m%s@%s:\033[40m\033[1;97m~%s\033[0m\033[1;92m", kullaniciAdi, anaBilgisayarAdi, dosyaKonum);
		printf(">>"); //prompt

		//bufferda tutulan veriler temizlendi
		fflush(stdout);
	}
}

//kulllanıcı karşılama ekranı
void karsilama() {

	printf("\033[0;106m_-----\033[0;105m------_---\033[0;104m--------\033[0;103m------\033[0;102------\033[0;101m_-----_-\n");
	printf("\033[0m\n");
	printf("\033[1;96m#######                                   ( ͡❛ ͜ʖ ͡❛) \n");
	printf("\033[1;95m#    #       ####  #####  #    # #####    --------------\n");
	printf("\033[1;94m    #       #    # #    # #    # #    #   HOŞ GELDİNİZ! \n");
	printf("\033[1;93m   #        #      #    # #    # #    #   --------------\n");
	printf("\033[1;92m  #         #  ### #####  #    # #####    Daha fazlası için\n");
	printf("\033[1;91m  #     ### #    # #   #  #    # #        Readme.md'yi okuyabilirsiniz\n");
	printf("\033[1;96m  #     ###  ####  #    #  ####  #        -----------------------------\n");
	printf("\n");
	printf("\033[1;95m                      #####                      \n");
	printf("\033[1;94m                      #####                      \n");
	printf("\n");
	printf("\033[1;93m     GRUP ÜYELERİ         ####      #### #    # ###### #      #      \n");
	printf("\033[1;92m     G181210107          #    #    #     #    # #      #      #      \n");
	printf("\033[1;91m     G181210023          #         ####  ###### #####  #      #      \n");
	printf("\033[1;96m     G181210029          #             # #    # #      #      #      \n");
	printf("\033[1;95m     G181210077          #    #   #    # #    # #      #      #      \n");
	printf("\033[1;94m     B191210401           ####     ####  #    # ###### ###### ###### \n");
	printf("\n");
	printf("\033[0;106m_-----\033[0;105m------_---\033[0;104m--------\033[0;103m------\033[0;102------\033[0;101m_-----_-\n");
	printf("\033[0m\n");
}

//Ctrl+D kontrolü için fonksiyon
int EofKontrol() {
	if (feof(stdin)) return 1;
	int c = getc(stdin);
	if (c == EOF) return 1;
	ungetc(c, stdin);
	return 0;
}

//Satırı okuyacak fonksiyon
char * satirOku() {
	/*
	dinamik bellek tahsisi yapıldı 512 karakterlik
	daha sonra char pointer tipine cast edildi satir değişkenine atandı
	*/
	char * satir = (char * ) malloc(sizeof(char) * 512);

	//Satır okurken karakterleri dolanacak karakter değişkeni
	char karakterDeger;

	//Satırı okurken karakterlerin indisini tutacak tamsayı değişken
	int indis = 0;

	//Arabellek boyutu
	int araBellekBoyutu = 512;

	//Eğer satır değeri yoksa ~ dinamik bellik tahsis edilemedi ise
	if (!satir) {
		printf("\nBellek Ayirma Hatasi \n");
		exit(EXIT_FAILURE); //hata varsa çık
	}

	//Ctrl+D işleyene kadar devam edecek işlem
	while (!EofKontrol()) {
		
		//Karakter değerini alalım
		karakterDeger = getchar();

		/*
		Eğer karakter değeri satır sonu veya dosya sonu ise
		okunan satır/komutun sonunu ifade etmeliyiz ve satırı geri döndürmeliyiz
		*/
		if (karakterDeger == EOF || karakterDeger == '\n') {
			satir[indis] = '\0';
			return satir;
		}
		//Eğer değil ise okuma işlemi devam ediyordur
		else {
			satir[indis] = karakterDeger;
		}

		indis++;
		//512 karakter kontrolü
		if (indis == 512) {
			fprintf(stderr, "Çok fazla karakter girdisi.\n");
			kontrol = true;
		}
	}
	printf("\n");
	exit(EXIT_SUCCESS);

}

/*
Çift işaretçi türünde okunan satırın ayırma işlemlerini gerçekleştirecek fonksiyon
satır işaretçisini parametre alır (okunan string değeri) onu ayırır dizi olarak geri döndürür
*/
char ** satirAyir(char * satir) {
	
	//Tüm elemanları tutan ~ çift işaretçi
	char ** elemanlar = (char ** ) malloc(sizeof(char * ) * 64);

	//Her bir elemanı tutan ~ işaretçi
	char * eleman;

	// ; ayırıcısı
	char ayiricilar[2] = ";";

	//Karakterin indisini tutacak değişken
	int indis = 0;

	//Arabellek boyutu
	int araBellekBoyutu = 64;

	//Eğer elemanlar yoksa, ayırma işlemi de yapılamaz!
	if (!elemanlar) {
		printf("\nBellek Ayirma Hatasi\n.");
		exit(EXIT_FAILURE);
	}

	//Satır, ayırıcılara göre ayrılır
	eleman = strtok(satir, ayiricilar);

	//Eleman NULL'u işaret edene dek
	while (eleman != NULL) {
		
		//Elemanlar dizisine işaretçi tip elemanı ata
		elemanlar[indis] = eleman;
		indis++; //Her atamada indis 1 artmalı


		//Bellek kontrolü için:
		//Ara bellek boyut sınırı ve aşma durumunda
		if (indis >= araBellekBoyutu) {
			
			//Ara bellek boyutuna kendisi kadar daha bir ekleme yapılır
			araBellekBoyutu += 64;

			//Bu ekleme realloc() ile dinamik olarak gerçekleşir
			elemanlar = (char ** ) realloc(elemanlar, araBellekBoyutu * sizeof(char * ));

			//Başarısız olma durumunda
			if (!satir) {
				printf("\nBellek Ayırma Hatasi\n");
				exit(EXIT_FAILURE);
			}

		}
		
		//Elemanı boşluklara bölme
		eleman = strtok(NULL, ayiricilar);
	}

	//Tek tek elemanlar, elemanlar dizine atandıktan sonra son indis NULL olur
	elemanlar[indis] = NULL;
	
	//Satırdan ayırılmış elemanlar geri döner
	return elemanlar;
}

/*
Çift işaretçi türünde okunan satırın ayırma işlemlerini gerçekleştirecek fonksiyon
satır işaretçisini parametre alır (okunan string değeri) onu ayırır dizi olarak geri döndürür
*/
char ** komutAyir(char * komut) {

	//Tüm elemanları tutan ~ çift işaretçi
	char ** elemanlar = (char ** ) malloc(sizeof(char * ) * 64);

	//Her bir elemanı tutan ~ işaretçi
	char * eleman;

	//Ayırıcı dizisi
	char ayiricilar[12] = " \t\n\r\a";

	//Karakterin indisini tutacak değişken
	int indis = 0;

	//Arabellek boyutu
	int araBellekBoyutu = 64;

	//Eğer elemanlar yoksa, ayırma işlemi de yapılamaz!
	if (!elemanlar) {
		printf("\nBellek Ayirma Hatasi\n.");
		exit(EXIT_FAILURE);
	}

	//Komutu ayırıcılara göre ayırır elemana atar
	eleman = strtok(komut, ayiricilar);

	//Eleman NULL'u işaret edene dek
	while (eleman != NULL) {
		//Elemanlar dizisine işaretçi tip elemanı ata
		elemanlar[indis] = eleman;
		indis++; //Her atamada indis 1 artmalı


		//Bellek kontrolü için:
		//Ara bellek boyut sınırı ve aşma durumunda
		if (indis >= araBellekBoyutu) {
			//Ara bellek boyutuna kendisi kadar daha bir ekleme yapılır
			araBellekBoyutu += 64;

			//Bu ekleme realloc() ile dinamik olarak gerçekleşir
			elemanlar = (char ** ) realloc(elemanlar, araBellekBoyutu * sizeof(char * ));

			//Başarısız olma durumunda
			if (!komut) {
				printf("\nBellek Ayırma Hatasi\n");
				exit(EXIT_FAILURE);
			}

		}

		//Elemanı boşluklara bölme
		eleman = strtok(NULL, ayiricilar);
	}

	//Tek tek elemanlar, elemanlar dizine atandıktan sonra son indis NULL olur
	elemanlar[indis] = NULL;

	//Satırdan ayırılmış elemanlar geri döner
	return elemanlar;
}

//Fonksiyon tanımları
int cdKomutu(char ** args);
int exitKomutu();

// Tanımlamalar
char * komutlar[] = {
	"cd",
	"exit"
};

//Komutların referansını içerir
int( * gomuluFonksiyon[])(char ** ) = {
	& cdKomutu,
	& exitKomutu
};

// Komut boyutlandırılması
int komutBoyut() 
{
	return sizeof(komutlar) / sizeof(char * );
}

// Dizin değiştirme komutu
int cdKomutu(char ** args) {
	if (args[1] == NULL) {
		printf("Kabuk: Beklenen argüman \"cd\"\n");
	} else {
		if (chdir(args[1]) != 0) {
			perror("Kabuk: ");
		}
	}
	return 1;
}

//Çıkış yapma komutu
int exitKomutu() {
	QUIT = 1;
	return 0;
}

// Çocuk process oluşturulup, komut çalıştırıldı
int komutCalistir(char ** args) {
	if (kontrol == true) return 1;
	pid_t pid, wpid;
	int status;
	// Komut quit ise kabuktan çıkar
	if (strcmp("quit", args[0]) == 0) {
		exit(EXIT_SUCCESS);
	}
	printf("Komut: %s \n", args[0]);
	pid = fork();
	if (pid == 0) {
		// Çocuk Processler İçin
		if (execvp(args[0], args) == -1) {
			fprintf(stderr, "Komut çalıştırılmadı. Komut yok veya komut yürütülemez.\n");
		}
		exit(EXIT_FAILURE);
	} else if (pid < 0) {
		perror("Child process oluşturma hatası.");
	} else {
	// Çalıştırılan komut sayısı toplam komut sayısına eşit olunca çocuk processleri beklemesi için
    if(i == komutSayisi)
    {
      do 
	  {
		//Çocuk processleri beklemesi için wait kullanıldı
        wpid = waitpid(pid, & status, WUNTRACED);
      } while ((wpid = wait(&status)) > 0);
    }
	}
	return 1;
}

// Gelen komutları çalıştırır
int kabukCalistir(char ** args) {
	if (args[0] == NULL) {
		// Boş komut
		return 1;
	}
	// Cd ya da exit komutu mu diye kontrol eder 
	for (int i = 0; i < komutBoyut(); i++) // numBuiltin() returns the number of builtin functions
	{
		// Cd veya exit komutu ise
		if (strcmp(args[0], komutlar[i]) == 0) 
			// gomuluFonksiyon içerisine gönderip geri döndürür
			return ( * gomuluFonksiyon[i])(args); 
	}

	//Argümanları parametre olarak verir komutCalistir çağırır
	return komutCalistir(args);
}

//İnteraktif modu işletecek olan fonksiyon
int interaktifMod() {

	char * satir = NULL;
	char ** args = NULL;
	char ** komutlar = NULL;

	while (QUIT == 0) {
		komutIstemi();
		satir = satirOku();
		komutlar = satirAyir(satir);
    
		//Komut sayısını bulmak için
		while (komutlar[komutSayisi] != NULL)
		{
		komutSayisi++;
		}
		
		//Komutlar sırasıyla alınıp çalıştırılır
		while (komutlar[i] != NULL) {
			args = komutAyir(komutlar[i]);
			i++;
			kabukCalistir(args);
		}

		kontrol = false;
		//Bellek serbest bırakıldı
		if (satir != NULL) free(satir);
		if (args != NULL) free(args);
		if (komutlar != NULL) free(komutlar);
		satir = NULL;
		args = NULL;
		komutlar = NULL;
		i = 0;
		komutSayisi = 0;
	}

	return 1;
}

//Batch mod'u işletecek olan fonksiyon
int batchMod(char dosyaAdi[100]) {
	FILE * fptr;
	char * satir = (char * ) malloc(sizeof(char) * 512);
	char ** args;
	size_t len = 512;
	char ** komutlar;
	fptr = fopen(dosyaAdi, "r");
	if (fptr == NULL) 
	{
		fprintf(stderr, "\nBatch dosyası mevcut değil veya açılamıyor.\n");
		return 1;
	}
	else 
	{
		printf("\nDosya açıldı. \n");
		while (getline( & satir, & len, fptr) != EOF) {
			printf("Satır : %s" , satir);
			komutlar = satirAyir(satir);
			
			//Komut sayısını bulmak için
			while (komutlar[komutSayisi] != NULL)
			{
				komutSayisi++;
			}
	
			//Komutlar sırasıyla alınıp çalıştırılır
			while (komutlar[i] != NULL) {
				args = komutAyir(komutlar[i]);
				i++;
				kabukCalistir(args);
			}
			i = 0;
      		komutSayisi = 0;
		}
	}
	//Bellek serbest bırakıldı
	free(args);
	free(komutlar);
	free(satir);
	fclose(fptr);
	return 1;
}

int main(int argc, char ** argv) {
	//Karşılama ekranı bastırılır
	karsilama();
	if (argc == 1)
		interaktifMod();
	else if (argc == 2)
		batchMod(argv[1]);
	else
		fprintf(stderr, "Yanlış sayıda komut satırı argümanı.\n");

	return EXIT_SUCCESS;
}