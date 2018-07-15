# Fork-Signal-SharedMemory
CSE344 - System Programming
HW04
DENIZ BABAT
131044013
Yazılan kod ödev de pdf'te anlatılanlara göre yaptım. Ödev, fork, shared memory,
semahore ve sinyal handle etme işlemleri ile yapıldı.
Wholesaler parent process, chef'ler ise child process'lerdir.Toplamda 2 shared
memory, 2 semaphore ve 6 child + 1 parent process olarak toplamda 7 process
kullanılmıştır.
1. Parent process(Wholesaler) malzemeleri 1. shared momery'e yazıyor ve
semaphore arttıriyor.
2. Child processler(chef) biri(en erken davranan) semaphore azaltır ve shared
memory e girer işine yarıyorsa alır cake'i yapar ve 2. shared memory'e yazar ve
2. semaphore arttırır ve kendi 1. semaphore'dan çıkar.
1. Eğer işine yaramazsa 1. semaphore artırır diğer cild processin girmesini
sağlar.
3. Wholesaler cake'i alır, satar ve 2. semaphore azaltır.
4. Böylece döngü devam eder.
NOT: hocam random fonksiyonu zaman bağlı olarak çalıştığı için ve process te çok
hızlı olduğu whosaler aynı malzemleri getiriyorsunuz gibi düşünebilirsiniz. Ama
farklı zamanlarda denedim random üretiyor.
