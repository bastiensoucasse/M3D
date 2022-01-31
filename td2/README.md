Bastien Soucasse – Mondes 3D

# TD 2 – 1er février 2022

## Calcul d'intersection

Par défaut, le programme compile correctement et affiche bien la boîte englobante de l'objet.

![TW – Box](renderings/tw_box.png)

Après implémentation de l'intersection des faces, on obtient une image cohérente affichant chaque face triangulaire de l'objet.

![TW — Faces](renderings/tw_normales_faces.png)

En calculant cette fois les normales par sommets au lieu des normales par faces, on obtient un rendu plus _smooth_.

![TW — Faces](renderings/tw_normales_sommets_tw503.png)

Cependant on se rend vite compte que le temps de calcul est très important lorsque l'on a un objet avec beaucoup de faces, ici 12576 au lieu de 503 précédemment.

![TW — Faces](renderings/tw_normales_sommets_tw.png)

Effectivement, le temps de _raytracing_ est passé de 1.57s à 25.48s.

## BVH

Cette partie fût beaucoup plus complexe à implémenter. Il n'y a pas vraiment eu de résultat visible pendant un long moment – ce qui n'aide déjà pas du tout soit dit en passant. Mais en plus de ça, malgré avoir acquis la théorie grâce au cours (et ça j'en suis sûr), ce fût très compliqué d'utiliser les librairies et le code fourni pour implémenter la BVH. J'ai passé approximativement 10h de cette semaine à débugguer mon code pour trouver comment le faire fonctionner, à l'aveuglette, à cause d'un manque d'indications, alors que le principe était complètement acquis.

### Fonctions `intersect` et `intersectNode`

Ça va on va dire…

### Fonction `buildNode`

Tout d'abord, la méthode `extend` a posé problème. Ne connaissant pas du tout la structure `AlignedBox3f` même si je comprends ce qu'elle représente, il fût compliqué de comprendre quoi passer en paramètre. J'ai fait le choix de passer les positions de chacun des sommets de la face.

De même, le reste de la fonction fût long à implémenter. Il ne s'agissait pas d'erreurs ou de bugs mais simplement de compréhension et d'implémentation difficiles. J'ai parfois du faire des choix en espérant que cela fonctionne par la suite. Parfois, un débuggage au `printf` était utile pour avancer, mais pas non plus déterminant.

Après ces efforts, j'ai pu (enfin) obtenir un premier rendu. Bien évidemment, ayant tout fait à l'aveuglette auparavent, le rendu ne pouvait pas être celui attendu.

![TW – BVH – Version 1](renderings/tw_bvh_1.png)

Ce résultat fût construit avec comme output :

```txt
Raytracing time: 15.6625s  -  nb triangle intersection: 1368806130
```

Autant dire que rien ne va, mis à part que j'ai réussi à avoir un résultat. Encore fallait-il trouver où, dans tout ce qui a été implémenté, pouvait-il y avoir des erreurs.

En outre, j'obtiens une erreur de segmentation lorsque je tente de lancer le programme avec `killeroo.scn` ou `killerooGlass.scn`.

En débuggant abondamment, j'obtiens un résultat qui montre que le split est la source du problème.

Cependant je ne sais pas comment le résoudre.

Allez tous vous faire foutre.

Cordialement.

🖕
