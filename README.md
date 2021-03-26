 C dilini kullanarak tasarladığımız kabukta ilk olarak batch mod ve interaktif mod olarak ikiye ayırdık.
Batch modda aldığımız txt dosyasını satır satır okuyarak bu satırları çeşitli fonksiyonlar ile önce ";" 
karakteri ile ayırıp ardından boşluk ve diğer ayırıcı karakterler ile parametrelerine ayırdık.
Ardından bu argümanları execvp fonksiyonuna vermesi için yazdığımız fonksiyonlara yolladık. Çeşitli hata 
kontrollerini(512 karakter kontrolü , argüman hatası , bellek tahsis hataları vb.) ve şartları gerçekleştirdik.

    İnteraktif mod içerisinde ise sonsuz bir döngü içerisinde kullanıcıdan bir girdi istedik.Ardından batch 
modda satır satır yaptığımız işlemi interaktif modda girdi girdi yaptık. Paralel çalışmayı fork , wait ve 
waitpid komutlarıyla sağladık. Yazdığımız kodda child prosesslerin execvp komutu ile girdiden aldığı 
komutları çalıştırmasını sağladık parent prosess ilede bir sonraki komuta döndük.


Özel Durumlar :

Satırları ve komutları ayırma işlemi;
Kodu ayırmak için hem interaktif hem de batch modda ayırıcılar tanımladık. Eleman işaretçisi Null gösterene dek while döngüsü
içerisinde dolaşarak atama yaptırdık her döngü sonunda strtok() fonksiyonu ile elemanı boşluklara böldük.

Komutun işlenip işlenmeyeceği;
Komutların çalıştırılacağı kısımda çocuk proses oluşturulurken ilk argüman geçersiz bir komutu ifade ediyorsa komut işlenmez,
kullanıcıya stderr ile hata mesajı yazdırılır, bu da execvp()'nin işleneceği koşul bloğu içerisinde sağlanır.

Kabuktan çıkış , Ctrl+D işlemi;
Satırı okurken karakter değerini alma işlemini dosya sonunu kontrol edene kadar çalışacak bir while döngüsü içerisinde sağladık.
Ctrl+D işlemi gerçekleşince de EOF döneceği için otomatik olarak temel fonksiyonlarımızdan olan satır okuma işlevi sonlanmış oldu
ve böylece kullanıcı kabuktan çıkış yaptı.

Boş komut gönderilmesi;
Kullanıcı boş satırda enter tuşuna bastığı zaman gelen girdi Null'a eşit ise (bu batch modda ";;" şeklinde gösterilir.) kabuk hiçbir
etkileşim göstermeden sonraki komutu almak için prompt'u yazdırarak girdi bekler.


Bilinen Hatalar Veya Sorunlar:
Projemizde kendi testlerimizin sonucunda herhangi bir hata veya sorun tespit edemedik. Tespit ettiğimiz hataların hepsini geliştirme
aşamasında çözümledik.
