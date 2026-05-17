For English see [README.en.md](README.en.md)
# Dépendances
Pour faire tourner ce programme vous aurez besoin des outils du paquet `qemu` , `gcc` et `nasm`

# Compilation
Après avoir satisfait les dépendances éxecuter `make build`

# Éxecution
Exécuter `make all` ou `make run`

# Résultat
Écran d'acceuil:

<img width="730" height="463" alt="titanOS1" src="https://github.com/user-attachments/assets/f6a70591-013e-4589-a29c-80e45694a97d" />

<img width="730" height="463" alt="titanOS2" src="https://github.com/user-attachments/assets/3feff538-8fbd-46b9-b58a-03676aa9b60c" />

# Commandes
### end
Arrête le cpu mais n'éteint pas la carte graphique
### clear 
Éfface l'écran
### version
Écris la version
### read
Affiche sous forme hexadécimal un secteur disque
### tick
Renvoie le nombre de ticks cpu depuis l'initialisation de l'horloge
### cat 
Est sensé afficher sous forme textuel un secteur disque
### page
Commande visant à tester le système d'allocation mémoire
### int 
Cause plus ou moins un plantage du ystème ( voulu )
### 2Dtest
Affiche une "image" composée de deux éléments distint
### hist 
Ne marche pas.
### fat_format
Inutile ( je ne suis même pas sûr qu'elle fonctionne )
### touppercase
Met en majuscule la chaine passée en argument
### tolowercase
Met en minuscule la chaine passée en argument
### video
Change le mode d'affichage de la carte graphique de text 3h à graphic 256 color linear 13h.
Notez que certaines commandes ne fonctionnent que dans l'un ou l'autre des modes
### term
cette commande permet d'ouvrir une fenêtre de terminal. ( Ne marche qu'en mode 13h )
# terminal
## fonctionnalités 
### variables
#### définition 
utiliser l'opérateur '=' comme ça: `clé=valeur`. ATTENTION ne pas entourer l'égal par des espaces
#### utilisation 
utiliser l'opérateur '$' de cette manière: `commande $clé`
### exécution en chaîne
utiliser l'opérateur '&&' pour enchaîner plusieurs commandes. Ex: `commande1 paramètre1 && commande2`. ( notez que cette fonctionnalité pourrait ne pas fonctionner correctement au-delà de 3 commandes )
## commandes
### echo
affiche la chaîne de caractères passée en argument 
### echolor
sous la forme de: `echolor <chaine> <nombre1> <nombre2>`
où nombre1 est la couleur de l'avant plan et nombre2 celle de l'arrière plan.
Note: 15 sera remplacé par la couleur du terminal et 0 par le fond du terminal
### palette 
affiche la palette de couleur
### snake
lance une partie de snake ( ZQSD pour les mouvements )
### game2048
( en cours ) lance une partie de 2048