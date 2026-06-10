# Symulator Farmy 2D

Symulator Farmy 2D to gra napisana w C++ z wykorzystaniem bibliotek SFML, tmxlite oraz JSON.
Gracz zarządza własną farmą: sadzi rośliny, podlewa je, zbiera plony, sprzedaje produkty i rozwija gospodarstwo.
Projekt korzysta z map przygotowanych w edytorze Tiled oraz z systemu stanów umożliwiającego obsługę wielu ekranów gry.

# Funkcje gry
- poruszanie się po mapie 2D
- animowany gracz (chodzenie, stanie)
- system pól uprawnych z etapami wzrostu
- sadzenie, podlewanie i zbieranie roślin
- sklep z kupowaniem nasion i sprzedażą plonów
- system złota i magazynu
- zapis i wczytywanie gry do pliku JSON
- menu główne, pauza, ekran pomocy, ekran startowy
- mini‑menu kontekstowe dla pól uprawnych
- renderowanie mapy Tiled (tmxlite)

# Technologie
- C++17
- SFML 2.6 (grafika, wejście, okno)
- tmxlite (obsługa map Tiled)
- JSON for Modern C++ (zapis i odczyt save.json)
- MinGW / g++ (kompilator)

# Kompilacja (Windows / MinGW)

"""
g++ src/main.cpp \
src/core/*.cpp \
src/states/*.cpp \
src/entities/*.cpp \
src/systems/*.cpp \
lib/tmxlite/src/*.cpp \
lib/tmxlite/src/detail/*.cpp \
lib/tmxlite/src/miniz.c \
-I include \
-I lib/tmxlite/include \
-I lib/tmxlite/src \
-L lib/sfml \
-lsfml-graphics -lsfml-window -lsfml-system \
-o bin/gra.exe
"""

# Moduły projektu

core/
Game  
Odpowiada za główną pętlę gry, obsługę okna SFML oraz zarządzanie stosami stanów. Koordynuje przełączanie ekranów, przetwarzanie wejścia i renderowanie. Pełni rolę centralnego kontrolera całej aplikacji.

State  
Abstrakcyjna klasa bazowa definiująca interfejs dla wszystkich stanów gry. Wymusza implementację metod handleInput, update i draw, co zapewnia spójność działania różnych ekranów.

SaveSystem  
Zajmuje się zapisem i odczytem danych gry do pliku JSON. Przechowuje informacje o graczu, złocie, roślinach i polach uprawnych. Podczas wczytywania odtwarza pełny stan rozgrywki.

states/
MenuState  
Wyświetla menu główne i obsługuje wybór opcji przez gracza. Odpowiada za przejście do innych stanów, takich jak gra, pomoc czy wyjście. Renderuje tło, tytuł i listę opcji.

StartState  
Ekran powitalny wyświetlany po uruchomieniu gry. Prezentuje podstawowe instrukcje i przechodzi do rozgrywki po wciśnięciu odpowiedniego klawisza.

PlayState  
Główny stan rozgrywki odpowiedzialny za wczytywanie mapy, obsługę gracza, kolizje i interakcje. Zarządza systemem roślin, kamerą oraz interfejsem użytkownika. Renderuje wszystkie elementy świata gry.

PauseState  
Wyświetla menu pauzy z opcjami wznowienia gry, zapisu, pomocy i powrotu do menu. Obsługuje nawigację i podświetlanie wybranej opcji.

HelpState  
Prezentuje ekran pomocy z opisem sterowania i podstawowych zasad gry. Umożliwia szybki powrót do poprzedniego stanu.

ShopState  
Odpowiada za interfejs sklepu, gdzie gracz może kupować nasiona i sprzedawać plony. Obsługuje logikę transakcji oraz wyświetlanie cen i magazynu.

MiniMenuState  
Kontekstowe mini‑menu pojawiające się po wejściu w interakcję z polem uprawnym. Umożliwia sadzenie, podlewanie i zbieranie roślin w zależności od aktualnego stanu pola.

entities/
Player  
Reprezentuje postać gracza wraz z animacjami ruchu i stania. Przechowuje pozycję, sprite oraz hitbox do kolizji. Aktualizuje animację w zależności od kierunku ruchu.

systems/
PlantSystem  
Zarządza wszystkimi roślinami i polami uprawnymi na mapie. Odpowiada za wzrost roślin, interakcje (sadzenie, podlewanie, zbieranie) oraz ich renderowanie. Wczytuje pola z mapy Tiled i aktualizuje ich stan w czasie.