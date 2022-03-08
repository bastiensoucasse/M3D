# Textures – TD6

Bastien Soucasse – M3D

## Plaçage de texture

1. Implémentation des textures.

    - Pour la texture de la Terre :

        ![renderings/earth.png](renderings/earth.png)

    - Pour la texture des nuages :

        ![renderings/clouds.png](renderings/clouds.png)

    - Pour la texture de la nuit :

        ![renderings/night.png](renderings/night.png)

2. Implémentation de la lumière.

    - Pour la texture de la Terre :

        ![renderings/earth-light.png](renderings/earth-light.png)

    - Pour la texture des nuages :

        ![renderings/clouds-light.png](renderings/clouds-light.png)

    - Pour la texture de la nuit :

        ![renderings/night-light.png](renderings/night-light.png)

3. Implémentation des mélanges de textures.

    - Pour la texture de la Terre et des nuages :

        ![renderings/earth-clouds-mix-light.png](renderings/earth-clouds-mix-light.png)

    - Pour la texture de la Terre, des nuages et de la nuit :

        ![renderings/earth-clouds-night-mix-light.png](renderings/earth-clouds-night-mix-light.png)

## Filtrage

1. Implémentation des filtres.

    - Comparaison de _mag_ :

        ![renderings/checkerboard-min-0-mag-0.png](renderings/checkerboard-min-0-mag-0.png)

        ![renderings/checkerboard-min-0-mag-1.png](renderings/checkerboard-min-0-mag-1.png)

        On constate que la zone à proximitée de la caméra est moins nette avec la deuxième version.

    - Comparaison de _min_ :

        ![renderings/checkerboard-min-0-mag-0.png](renderings/checkerboard-min-0-mag-0.png)

        ![renderings/checkerboard-min-1-mag-0.png](renderings/checkerboard-min-1-mag-0.png)

        ![renderings/checkerboard-min-2-mag-0.png](renderings/checkerboard-min-2-mag-0.png)

        ![renderings/checkerboard-min-3-mag-0.png](renderings/checkerboard-min-3-mag-0.png)

        ![renderings/checkerboard-min-4-mag-0.png](renderings/checkerboard-min-4-mag-0.png)

        ![renderings/checkerboard-min-5-mag-0.png](renderings/checkerboard-min-5-mag-0.png)

        On constate que la zone éloignée de la caméra est plus ou moins nette selon la valeur choisie.

2. Implémentation de la MipMap personnalisée.

    - Comparaison de _mag_ :

        ![renderings/cow-mipmap-min-0-mag-0.png](renderings/cow-mipmap-min-0-mag-0.png)

        ![renderings/cow-mipmap-min-0-mag-1.png](renderings/cow-mipmap-min-0-mag-1.png)

        On ne constate pas vraiment de différence.

    - Comparaison de _min_ :

        ![renderings/cow-mipmap-min-0-mag-0.png](renderings/cow-mipmap-min-0-mag-0.png)

        ![renderings/cow-mipmap-min-1-mag-0.png](renderings/cow-mipmap-min-1-mag-0.png)

        ![renderings/cow-mipmap-min-2-mag-0.png](renderings/cow-mipmap-min-2-mag-0.png)

        ![renderings/cow-mipmap-min-3-mag-0.png](renderings/cow-mipmap-min-3-mag-0.png)

        ![renderings/cow-mipmap-min-4-mag-0.png](renderings/cow-mipmap-min-4-mag-0.png)

        ![renderings/cow-mipmap-min-5-mag-0.png](renderings/cow-mipmap-min-5-mag-0.png)

        Les différences ne sont pas énormes mais on constate que les MipMap relevées varient selon ce paramètre. 
