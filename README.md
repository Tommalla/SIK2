SIK2
====

Second project for Computer Networks


Task in Polish
----
Termin oddania: 9 kwietnia 2014, godzina 19:00
                (liczy się czas na serwerze SVN)

1. Napisać programy klienta (mierniczyc) i serwera (mierniczys), które
   działają w następujący sposób:   
   a) serwer pobiera z wiersza poleceń numer portu, a następnie   
   - nasłuchuje na gnieździe TCP na połączenia,
   - po odebraniu połączenia pobiera z niego numer portu UDP i dane,
     aż do zakończenia połaczenia przez klienta.
   - następnie przesyła z powrotem do klienta po UDP liczbę otrzymanych 
     oktetów danych (nie licząc numeru portu).   
   b) klient pobiera z wiersza poleceń numer portu UDP, numer portu TCP,    
     oraz adres serwera (w takiej dokładnie kolejności), a następnie
   - otwiera port UDP o numerze podanym w wierszu poleceń,
   - łączy się z serwerem za pomocą TCP na podany w wierszu poleceń numer
     portu i wysyła numer portu UDP oraz dane wczytane ze standardowego 
     wejścia, po czym kończy połączenie,
   - odbiera po UDP liczbę oktetów, ale tylko od serwera, do
     którego wysłał dane,
   - wypisuje otrzymany wynik na standardowe wyjście.
2. Numer portu UDP to liczba 16-bitowa. Liczba oktetów to liczba 32-bitowa.
   Liczby przesyłamy w systemie bigendian.
3. Stworzoną parę plików z rozwiązaniem należy zaopatrzyć w plik Makefile 
   zapewniający automatyczną kompilację.
4. Pliki źródłowe mierniczyc.c, mierniczys.c oraz Makefile należy umieścić
   na serwerze SVN

   https://svn.mimuw.edu.pl/repos/SIK/

   w katalogu

   grupaN/zadanie2/ab123456

   gdzie grupaN oznacza grupę, do której przynależy student, zaś ab123456
   to jego standardowy login wg schematu: inicjały, nr indeksu, używany
   na maszynach wydziału. Można też umieścić tam inne pliki potrzebne do
   skompilowania i uruchomienia programu, jeśli to jest konieczne.
