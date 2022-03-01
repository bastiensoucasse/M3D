# Textures - TD 6

Bastien Soucasse – Mondes 3D

## Plaçage de texture

On va travailler sur l'objet `sphere.obj`, il faut donc tout d'abord modifier `Viewer::init()` pour prendre le bon objet.

1. Ajouter en entrée du _Vertex Shader_ une variable représentant les coordonnées de la position des textures `vtx_texcoords`, qui sera passée au _Fragment Shader_ (comme `vtx_color`). Ajouter son initialisation dans `Mesh::draw()`. Attention au dernier paramètre de `glVertexAttribPointer()` qui représente la position du paramètre dans le fichier, on se décale de deux _Vector3f_ (position et normale), et d'un _Vector4f_ (couleur).

2. Ajouter à la classe `Viewer` un attribut `GLuint _texID0` qui correspond à **l'identifiant de texture** et l'initialiser dans `Viewer::init()` (juste avant `glEnable(GL_DEPTH_TEST)`).

    ```cpp
    _texID0 = SOIL_load_OGL_texture(DATA_DIR "/textures/earth.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y | SOIL_FLAG_MIPMAPS);
    ```

    Bon à savoir : _SOIL2_ permet grâce à sa fonction `SOIL_load_OGL_texture()` de faire toutes les étapes nécessaires à la création d'une texture en OpenGL automatiquement.

3. Ajouter au _Fragment Shader_ une variable uniforme de type `sampler2D` qui représente la texture à utiliser, et remplacer la couleur de sortie de ce _Shader_ en fonction.

    ```glsl
    out_color = vec4(texture(<sampler2D_utilisé>, var_texcoords).rgb, 1);
    ```

4. Dans `Viewer::drawScene()`, activer la première unité de texture (0) avec `glActiveTexture(GL_TEXTURE0)`, faire le lien avec l'identifiant de texture mis en place au point 2 avec `glBindTexture(GL_TEXTURE_2D, _texID0)`, puis la transmettre au _Fragment Shader_ grâce à la variable du point 3 avec `glUniform1i(_shader.getUniformLocation("tex0"), 0)` (tex0 correspondant au nom du _Sampler2D_ dans le _Fragment Shader_ et 0 correspondant à l'unité de texture à transmettre).

    Toujours dans `Viewer::drawScene()`, mais cette fois après avoir dessiné la scène, activer la même unité de texture avec `glActiveTexture(GL_TEXTURE0)`, et lui retirer le lien avec l'identifiant de texture avec `glBindTexture(GL_TEXTURE_2D, 0)`.

5. Il est alors possible de lancer le projet et de visualiser l'objet texturé : la Terre (woaw).

6. Ajouter la prise en compte de la lumière ajoutée au TD précédent. Pour cela, commenter (`//`) la sortie `out_color` que l'on vient de faire, on va partir de celle du TD précédent qui utilise `blinn()`.

    ```glsl
    out_color = vec4(blinn(normalize(n), normalize(v), l, vCol, sCol, s), 1);
    ```

    On va seulement remplacer la couleur de l'objet par la couleur de la texture : `texture(<sampler2D_utilisé>, var_texcoords).rgb`.

7. Il est alors possible de lancer le projet et de visualiser la Terre éclairée par la lumière ajoutée et ombragée à l'opposée.

8. Comme précédement, ajouter deux **indentifiants de textures** et les initialiser pour `earth_clouds.jpg` et `earth_night`, ajouter deux variables uniformes de type `sampler2D` au _Fragment Shader_, et ajouter l'activation, le lien, la transmission au _Fragment Shader_, et le délien, des **identifiants de textures**, aux **unités de textures** 1 et 2.

    Il est alors possible (pour tester) d'afficher chaque texture individuellement en modifiant le _Sampler2D_ utilisé par le _Fragment Shader_ dans l'appel à `blinn()` dans la définition de `out_color`.

9. On va commencer par mélanger la texture de la Terre et la texture des nuages afin d'avoir une texture finale de la Terre avec nuages. Dans l'appel à `blinn()` on utilise la couleur de texture en guise de couleur diffuse/ambiante. On va donc désormais le remplacer par l'appel à la fonction `mix()` de GLSL, qui prend en paramètre la première texture (donc sans le `.rgb`), la deuxième texture (les nuages), et le premier canal (`.x`) de la deuxième texture (je suis désolé j'ai pas compris pourquoi). On obtient un _vec4_ il faut donc ne pas oublier de passer uniquement les coordonnées _x_, _y_ et _z_.

10. Il est alors possible de lancer le projet et de visualiser la Terre illuminée, avec des nuages en plus.
