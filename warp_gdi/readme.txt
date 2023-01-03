>> cmake project aus dem fotoalbum erstellt <<

12/29
- name "stolen"
- fortschrittsleiste, die in 3d durch mauern sprengt
+ xm-player: SDL + libxmp, nope: SDL + IBXM

12/28
+ original font from demozoo: https://demozoo.org/sceners/52356/
+ classes "img" und "sprite"
+ amiga balls

Natalia "Lego für Programmierer"
2do:
+ add FPS counter (OR: performance counter)
(-) faster double buffering, s. http://www.winprog.org/tutorial/animation.html
was soll alles in dem demo enthalten sein?
- scroller
- try on an older (than win 11) pc
- musik spectrum analyzer
- vectorgrafik
- amiga ball als vektorgrafik (i.e. spinning)
- 50 FPS
- grafiken etc. zu source code convertieren
+ xm-player: SDL + libxmp, nope: SDL + IBXM


9/24
+ renamed "fotoalbum" to "bobs"
- turrican functionality nachbauen (SDL)
- class dib_img erstellen -> davon img und sprite ableiten
+ owl ausschneiden basierend auf timer
+ transparent blitten, http://www.winprog.org/tutorial/transparency.html
  -> mask blit nicely explained: https://en.wikipedia.org/wiki/Bit_blit
+ die owl wurde nicht geblittet, ah, erst muss das hdcMem erstellt werden!
+ owl als png -> laden mit stbi!!
+ owl bitmap von https://amiga.lychesis.net/artist/FranckSauer/Agony_Owl.html
- cmake project aus dem fotoalbum erstellt
