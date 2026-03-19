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