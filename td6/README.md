# Textures - TD 6

LC – Mondes 3D

## Plaçage de texture

On va travailler sur l'objet `sphere.obj`, il faut donc tout d'abord s'assurer que `Viewer::init()` charge le bon objet.

1. Ajouter en entrée du _Vertex Shader_ une variable représentant les coordonnées de la position des textures `vtx_texcoords`, qui sera passée au _Fragment Shader_ (comme `vtx_color`). Ajouter son initialisation dans `Mesh::draw()`. Attention au dernier paramètre de `glVertexAttribPointer()` qui représente la position du paramètre dans le fichier, on se décale de deux _Vector3f_ (position et normale), et d'un _Vector4f_ (couleur).

2. Ajouter à la classe `Viewer` un attribut `GLuint _texID0` qui correspond à **l'identifiant de texture** (c'est-à-dire l'objet représentant la texture même, à ne pas confondre avec unité de texture que l'on voit au point 4) et l'initialiser dans `Viewer::init()` (juste avant `glEnable(GL_DEPTH_TEST)`).

    ```cpp
    _texID0 = SOIL_load_OGL_texture(DATA_DIR "/textures/earth.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
    ```

    **Bon à savoir :**\
    _SOIL2_ permet grâce à sa fonction `SOIL_load_OGL_texture()` de faire toutes les étapes nécessaires à la création d'un **identifiant de texture** en OpenGL automatiquement (présentées dans l'introduction du TD sur Moodle).

3. Ajouter au _Fragment Shader_ une variable uniforme de type `sampler2D` qui représente la texture à utiliser, et remplacer la couleur de sortie de ce _Shader_ en fonction.

    ```glsl
    out_color = vec4(texture(<sampler2D_utilisé>, var_texcoords).rgb, 1);
    ```

4. Dans `Viewer::drawScene()`, activer la première **unité de texture** (c'est-à-dire l'objet faisant le lien entre l'identifiant de texture et le _Fragment Shader_) avec `glActiveTexture(GL_TEXTURE0)`, faire le lien avec l'identifiant de texture mis en place au point 2 avec `glBindTexture(GL_TEXTURE_2D, _texID0)`, puis la transmettre au _Fragment Shader_ grâce à la variable du point 3 avec `glUniform1i(_shader.getUniformLocation("tex0"), 0)` (tex0 correspondant au nom du _Sampler2D_ dans le _Fragment Shader_ et 0 correspondant à l'unité de texture à transmettre).

    Toujours dans `Viewer::drawScene()`, mais cette fois après avoir dessiné la scène, activer la même unité de texture avec `glActiveTexture(GL_TEXTURE0)`, et lui retirer le lien avec l'identifiant de texture avec `glBindTexture(GL_TEXTURE_2D, 0)`.

5. Il est alors possible de lancer le projet et de visualiser l'objet texturé : la Terre (woaw).

6. Ajouter la prise en compte de la lumière ajoutée au TD précédent. Pour cela, commenter (`//`) la sortie `out_color` que l'on vient de faire, on va partir de celle du TD précédent qui utilise `blinn()`.

    ```glsl
    out_color = vec4(blinn(normalize(n), normalize(v), l, vCol, sCol, s), 1);
    ```

    On va seulement remplacer la couleur de l'objet par la couleur de la texture : `texture(<sampler2D_utilisé>, var_texcoords).rgb`.

7. Il est alors possible de lancer le projet et de visualiser la Terre éclairée par la lumière ajoutée et ombragée à l'opposée.

8. Comme précédement, ajouter deux indentifiants de textures et les initialiser pour `earth_clouds.jpg` et `earth_night`, ajouter deux variables uniformes de type `sampler2D` au _Fragment Shader_, et ajouter l'activation, le lien, la transmission au _Fragment Shader_, et le délien, des identifiants de textures, aux unités de textures 1 et 2.

    Il est alors possible (pour tester) d'afficher chaque texture individuellement en modifiant le _Sampler2D_ utilisé par le _Fragment Shader_ dans l'appel à `blinn()` dans la définition de `out_color`.

9. On va commencer par mélanger la texture de la Terre et la texture des nuages afin d'avoir une texture finale de la Terre avec nuages. Dans l'appel à `blinn()`, on utilise une couleur diffuse/ambiante. On va désormais la remplacer par la couleur renvoyée par la fonction `mix()` de GLSL. Celle-ci prend en paramètre la première couleur (texture 0 sans le `.rgb` car on veut garder le _alpha channel_), la deuxième couleur (pareil pour la texture 1), et le premier canal (`.x`) de la deuxième couleur. (Je suis désolé j'ai très pas compris pourquoi, mais en tous cas c'est la valeur qui représente l'importance de la deuxième texture sur la première. Donc en gros si t'as un nuage important (pixel blanc) tu affiches que le nuage, si tu as un nuage moyen (pixel gris) tu mélanges à 50-50, et si tu as aucun nuage (pixel noir) tu affiches la que la Terre.) On obtient un _vec4_ il faut donc ne pas oublier de passer uniquement les coordonnées _x_, _y_ et _z_.

10. Il est alors possible de lancer le projet et de visualiser la Terre illuminée, avec des nuages en plus.

11. De la même manière, mélanger cette fois la toisième texture et le mélange effectué précedémment, avec comme valeur d'importance le terme diffus.

12. Il est alors possible de lancer le projet et de visualiser la Terre illuminée, avec des nuages, et les lumières de la nuit dans les zones d'ombres en plus.

**N.B.** :\
Il semble important de noter que sur les images du sujet sur _Moodle_, j'ai l'impression qu'il n'y a pas la lumière spéculaire. Peut-être faut-il donc enlever la partie spéculaire en mettant sont terme à 0 (ou bien en commantant tout le calcul etc.).

## Filtrage

On va travailler sur l'objet `plane.obj`, il faut donc tout d'abord modifier `Viewer::init()` pour charger le bon objet.

Il faut également changer la texture utilisée. Pour cela, dans `Viewer::init()`, commenter l'initialisation des trois identifiants de texture utilisés précédemment, et faire à la place l'initialisation du premier seulement (on n'utilise qu'une seule texture) avec `checkerboard.png` (attention à l'extention, ne pas faire comme moi 🙃).

De même, dans `Viewer::draw()`, commenter l'initialisation des unités de texture 1 et 2 (leur activation, leur lien à l'identifiant de texture, et leur passage au _Fragment Shader_), ainsi que la libération après la phase d'affichage (leur activation, et leur délien à l'identifiant de texture). On n'utilise ainsi plus qu'une seule unité de texture.

**N.B.** :\
Il n'est pas nécéssaire de modifier l'initialisation de la première unité de texture car celle-ci sert juste de lien entre l'identifiant de texture déjà modifié dans `Viewer::init()` et le _Fragment Shader_, elle est donc toujours valide.

Il faut égalment adapter le _Fragment Shader_ pour qu'il ne prenne en compte que la première texture (plus de `mix()` aussi du coup, on a seulement cette texture en guise de couleur de l'objet).

Vu qu'il s'agit d'un plan, on peut pas visualiser quoi que se soit en étant en face. On va donc modifier dans `Viewer::init()` l'appel à `Camera::lookAt()` avec par exemple la version suivante.

```cpp
_cam.lookAt(Vector3f(-4.f, 1.f, 4.f), Vector3f(0.f, 0.f, 0.f), Vector3f(0.f, 1.f, 0.f));
```

On peut aussi (le cas échéant, genre si tu utilises la correction partielle du TD 5 fournie) désactiver la rotation automatique en initialisant le paramètre `_rotate` à `false` dans le constructeur de `Viewer`.

1. Ajouter les attributs suivant à la classe Viewer.

    ```cpp
    GLuint _samplerId;              // Sampler.
    GLenum _minFilter = GL_NEAREST; // Minification.
    GLenum _magFilter = GL_NEAREST; // Magnification.
    ```

2. Dans `Viewer::init()`, initialiser le sampler `_samplerId` ainsi ajoutée, à l'aide de la fonction `glGenSamplers()`.

3. Dans `Viewer::draw()` lors de la phase d'affichage (avant l'appel à `Mesh::draw()`), activer ce sampler avec `glBindSampler()` sur la première unité, puis définir les paramètres de filtrage (`GL_TEXTURE_MIN_FILTER` et `GL_TEXTURE_MAG_FILTER`) grâce à `glSamplerParameteri()`, en forçant le _cast_ en `GLint` sur leurs valeurs.

4. On veut pouvoir changer les paramètres du sampler durant l'exécution. Pour cela on va tout d'abord changer la déclaration/définition des deux paramètres (`_minFilter` et `_magFilter`) dans la classe `Viewer` en une liste des valeurs possibles et un indice de la valeur courante dans cette liste, pour chaque paramètre.

    ```cpp
    GLenum _minFilters[6] = { GL_NEAREST, GL_LINEAR, GL_NEAREST_MIPMAP_NEAREST, GL_LINEAR_MIPMAP_NEAREST, GL_NEAREST_MIPMAP_LINEAR, GL_LINEAR_MIPMAP_LINEAR };
    GLenum _magFilters[2] = { GL_NEAREST, GL_LINEAR };
    int _minFilter = 1;
    int _magFilter = 1;
    ```

    Adapter en conséquence les deux appels à `glSamplerParameteri()` dans `Viewer::drawScene()` afin d'utiliser non plus les variables `_minFilter` et `_magFilter` directement mais les paramètres désignés par ces indices.

    Dans `Viewer::keyPressed()`, assigner des touches pour modifier les variables `_minFilter` et `_magFilter` (en les incrémentant et/ou décrémentant).

    Il est maintenant possible de tester les différentes combinaisons de paramètres avec les touches. En ayant légèrement tourné le plan pour bien voir les différences, j'ai créé une série d'images `checkerboard-[…].png` dans le dossier `lc-outputs/filters`.

Pour la suite, on va changer le modèle utilisé pour `cow.obj`, ainsi que la texture unique pour `cow.jpg`. Il va également falloir enlever le _flag_ `SOIL_FLAG_MIPMAPS` (ainsi que le symbole `|` qui le précède) dans le chargement de cette texture.

On peut ensuite importer le code donné dans `Viewer::init()` avant l'appel à `glGenSamplers()`, et lancer pour visualiser le résultat. Encode une fois, j'ai créé un série d'images `cow-mipmap-[…].png` dans le dossier `lc-outputs/filters`. Il faut aussi répondre dans ton README.md à la question « Qu'en concluez-vous sur le fonctionnement du mip-mapping ? » mais pour le coup j'en ai aucune idée…

Voilà c'est fini, désolé pour les erreurs éventuelles, les trucs pas assez précis ou potentiellement trop guidés, les potentiels bugs non détectés, et les images pas forcément justes si j'ai pas tout bon mais voilà où j'en suis et ce que je pense être correct. J'ai fait de mon mieux pour te faire gagner un peu de temps et moins chier que le sujet de base ultra relou, j'espère tellement que ça te sera utile.

Bisous bébé ❤️
