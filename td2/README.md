Bastien Soucasse – Mondes 3D

# TD 2 – 1er février 2022

## Calcul d’intersection

Par défaut, le programme compile correctement et affiche bien la boîte englobante de l’objet.

![TW – Box](renderings/tw_box.png)

Après implémentation de l’intersection des faces, on obtient une image cohérente affichant chaque face triangulaire de l’objet.

![TW — Faces](renderings/tw_normales_faces.png)

En calculant cette fois les normales par sommets au lieu des normales par faces, on obtient un rendu plus _smooth_.

![TW — Faces](renderings/tw_normales_sommets_tw503.png)

Cependant on se rend vite compte que le temps de calcul est très important lorsque l’on a un objet avec beaucoup de faces, ici 12576 au lieu de 503 précédemment.

![TW — Faces](renderings/tw_normales_sommets_tw.png)

Effectivement, le temps de _raytracing_ est passé de 1.57s à 25.48s.

## BVH

Cette partie fut beaucoup plus complexe à implémenter. Il n’y a pas vraiment eu de résultat visible pendant un long moment. Mais en plus de ça, malgré avoir acquis la théorie grâce au cours, ce fut très compliqué d’utiliser les librairies et le code fourni pour implémenter la BVH. J’ai passé approximativement 10h de cette semaine à débugguer mon code pour trouver comment le faire fonctionner à l’aveuglette.

### Fonctions `intersect` et `intersectNode`

Ces deux méthodes ont été implémentées avec une certaine logique appliquée mais ne peuvent être testées avant que la suivante ne fonctionne.

Le principe étant de tester si la boîte englobante maximale est en intersection avec le rayon, si non ça ne sert à rien d’aller plus loin, si oui alors on lance un appel récursif sur les deux boites inférieures, et ainsi de suite. Si on atteint un feuille, on lance alors `intersectFace` sur chacune des faces de la feuille. On peut alors voir s’il y a intersection ou non. 

### Fonction `buildNode`

Tout d’abord, la méthode `extend` a posé problème. Ne connaissant pas du tout la structure `AlignedBox3f` même si je comprends ce qu’elle représente, il fut compliqué de comprendre quoi passer en paramètre. J’ai fait le choix de passer les positions de chacun des sommets de la face.

De même, le reste de la fonction fut long à implémenter. Il ne s’agissait pas d’erreurs ou de bugs mais simplement de compréhension et d’implémentation difficiles. J’ai parfois du faire des choix en espérant que cela fonctionne par la suite. Parfois, un débuggage au `printf` était utile pour avancer, mais pas non plus déterminant.

Après ces efforts, j’ai pu (enfin) obtenir un premier rendu. Bien évidemment, ayant tout fait à l’aveuglette auparavent, le rendu ne pouvait pas être celui attendu.

![TW – BVH – Version 1](renderings/tw_bvh_1.png)

Ce résultat fut construit avec comme output :

```txt
Raytracing time: 15.6625s  -  nb triangle intersection: 1368806130
```

Autant dire que rien ne va, mis à part que j’ai réussi à avoir un résultat. Encore fallait-il trouver où, dans tout ce qui a été implémenté, pouvait-il y avoir des erreurs.

En outre, j’obtiens une erreur de segmentation lorsque je tente de lancer le programme avec `killeroo.scn` ou `killerooGlass.scn`.

En débuggant abondamment, j’obtiens un résultat qui montre que le split est la source du problème.

Je sais que l’erreur provient de l’appel à `split`. En effet, seulement 8 nœuds sont créés. Très vite, le _split_ crée un enfant vide et un aussi plein de que le parent, et est donc considéré comme inutile. Le nœud sur lequel le _split_ est appliqué est alors défini comme feuille (sinon il va juste créer un enfant vide, et un égal au parent, celui-ci va créer un enfant vide, et un égal à lui-même, et ainsi de suite jusqu’à atteindre la récursion maximale, la feuille sera alors exactement la même que celle mise en place ici).

Cependant je ne sais pas comment le résoudre. Il m’est impossible d’aller plus loin malgré tous mes efforts et toutes les recherches et demandes d’aides auprès d’autres personnes et professeurs.
