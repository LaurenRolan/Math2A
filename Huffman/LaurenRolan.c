//Lauren Rolan

/* Fichier source initial pour le TP sur le codage de Huffman (Theorie de
l'information en 2Ainformatique). Ce fichier doit etre modifié et renommé. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h> 

#define NB_max 256

typedef struct noeud{
  unsigned char symbole ;
  int nb_occurences ;
  float frequence ;
  struct noeud *f_gauche, *f_droit ;
} noeud ;


/* Retourne un nouveau noeud initialisé avec les paramètres donnés en entrée */
noeud *nouveau_noeud(unsigned char symbole, int nb_occurences, noeud *gauche, noeud *droite){
  noeud *nouveau = (noeud *) malloc(sizeof(noeud)) ;
  nouveau -> symbole = symbole ;
  nouveau -> nb_occurences = nb_occurences ;
  nouveau -> f_gauche = gauche ;
  nouveau -> f_droit = droite ;
  return nouveau;					
}

/* Remplit le tableau de noeuds "table" avec les caractères du fichier "fichier", 
leur nombre d'occurence et retourne le nombre de caractères du fichier */ 
int initialiser_table(char *fichier, noeud *table[NB_max]){
  unsigned char c ;
  int n = 0 ;
  FILE *f ;

  f = fopen(fichier, "r") ;
  while (fscanf(f, "%c", &c) != EOF){
      table[c] -> nb_occurences++ ; 
      n ++ ;
    }
  fclose(f) ;
  return n ;
}

/* Insere le noeud N dans le tableau de noeuds "table" trié par ordre 
   décroissant entre les indices  0 à l - 1 en respectant l'ordre décroissant */
void inserer_noeud(noeud *table[NB_max], noeud *N, int l){
  int i = l ;

  while ((i >= 1) && (table[i - 1]->nb_occurences < N->nb_occurences)) {
      table[i] = table[i - 1] ;
      i = i - 1 ;
    }
  table[i] = N ;
}

/* Trie le tableau de noeud "table" par order décroissant en supprimant les
occurences qui sont nulles et en renvoyant le nombre d'occurences non nulles. */
int trier_table(noeud *table[NB_max]){
  int i, n = 0 ;

  for (i = 1 ; i < NB_max ; i++) 
    if (table[i]->nb_occurences > 0) {
	inserer_noeud(table, table[i], n) ;
	n++ ; 
      }
    else free(table[i]) ;
  return n ;
}

/* Complete la table avec le champs frequence de chaque noeud, correspondant au 
nombre d'occurence et retourne le nombre total de symbole non nul, avec multiplicité 
(a partir d'une table de N symboles non nul) */
int remplir_frequence_table(noeud *table[NB_max], int N){
  int i, n = 0 ;
  float nb ;

  for (i = 0 ; i < N ; i++)
	n = n + (table[i]->nb_occurences) ;
  for (i = 0 ; i < N ; i++){
	nb = table[i]->nb_occurences ;
	table[i]->frequence = nb/n ;
  }
  return n ;
}

/*Affiche à l'écran le contenu de la table "table" (de n noeuds) */ 
void afficher_table(noeud *table[NB_max], int n){
int i ;
printf("\nTable :\n");
  for (i = 0 ; i < n ; i++)
	printf("car%d : %c, nb : %d et freq :%.4f\n", i, table[i]->symbole, table[i]->nb_occurences, table[i]->frequence) ;
}  


/* renvoit la hauteur d'un arbre binaire donné*/
int calcul_hauteur_arbre(noeud *arbre) {
  int abr_g, abr_d;
  if (arbre == NULL) return -1;
  abr_g = calcul_hauteur_arbre(arbre -> f_gauche);
  abr_d = calcul_hauteur_arbre(arbre -> f_droit);
  if (abr_g >= abr_d) return abr_g +1;
  	else return abr_d +1;
}


/* Remplit la table "code" avec le codage de Huffman de chaque element
definit dans l'arbre correspondant de hauteur h */
void codage(noeud *arbre, char **code, char *s, int h) {
  unsigned char symbole ;

  if (arbre -> f_gauche == NULL){
      symbole = arbre->symbole ;
      s[h] = '\0';
      code[symbole] = (char *) malloc((h+1) * sizeof(char));
      strcpy(code[symbole], s); 
    }
  else {
      s[h] = '0';
      codage(arbre->f_gauche, code, s, h+1);
      s[h] = '1';
      codage(arbre->f_droit, code, s, h+1);
    }
}

/* Affiche le contenu de la table "code" vers la sortie (fichier ou sortie standard stdout) */
void afficher_codage(FILE  *f, char **code) {
  int i ;

  for (i = 0; i < NB_max ; i++)  
    if  (code[i] != NULL) 
	fprintf(f, "%c (%d) : %s\n", i, i, code[i]);
}

/* Retourne le nombre de 0 et de 1 correspondant au codage dans le texte codé 
   des lettres contenus dans arbre de hauteur h */
int nb_bits(noeud *arbre, int h)
{
  if (arbre -> f_gauche == NULL) 
	return h * arbre->nb_occurences ;
  return nb_bits(arbre->f_gauche, h + 1) + nb_bits(arbre->f_droit, h+1) ;
}

/*Retourne le nombre moyen de bits du code*/
double nb_moyen_bits(noeud *arbre, int h)
{
  if (arbre -> f_gauche == NULL) 
      return h * arbre->frequence;
	
  return (nb_moyen_bits(arbre->f_gauche, h + 1) + nb_moyen_bits(arbre->f_droit, h+1));
}


noeud *creer_arbre(noeud *table[NB_max], int nonNuls)
{
	int i = nonNuls - 1;
	while(i > 0)
	{
		noeud *feuille; 
		feuille = nouveau_noeud('.', table[i]->nb_occurences + table[i-1]->nb_occurences, table[i-1], table[i]);
		inserer_noeud(table, feuille, i);
		i--;
	}
	return table[0];
}

/******************************************************************************/

int main(int argc, char **argv) {
  noeud *table[NB_max], *arbre ;
  char *code[NB_max], *s ;
  int i, l, n, h, nb_total, lc ;
  double entropie=0;

  for (i = 0 ; i < NB_max ; i++) table[i] = nouveau_noeud(i, 0, NULL, NULL) ;
  l = initialiser_table(argv[1], table) ;
  n = trier_table(table) ;
  nb_total = remplir_frequence_table(table, n) ;
  afficher_table(table, n) ;

// Question 1 : calcul de l'entropie binaire de table
    for (i = 0; i < n; i++)
        entropie += table[i]->frequence * (log(table[i]->frequence) / log(2));
    entropie = - entropie;
  printf("%.2f\n", entropie);
  /*
  	Question 1
	Dans un texte petit, on peut dire que les lettres sont quasiment équiprobables
	comme un texte aléatoire. Dans un grand texte, par contre, les lettres ont une
	distribuition d'accord la langue qui il est écrit, et donc, ont probabilités variées.
  */

// Question 2 : construction de l'arbre de Huffman (retourne le noeud *arbre)
	arbre = creer_arbre(table, n);

/* Question 3 : calcul et affichage du codage :
le passage suivant est à décommenter après la question 2 :
*/
  h = calcul_hauteur_arbre(arbre) ;
  printf("hmax = %d ",h) ;
  for (i = 0 ; i < NB_max ; i++) code[i] = NULL ;
  s = (char *) malloc((h+1) * sizeof(char)) ;
  codage(arbre, code, s, 0);
  afficher_codage(stdout, code);

  /*
	Question 3
	On peut vérifier dans l'analyse du fichier "Huffman.txt" qui la lettre
	plus fréquente est "e", donc, elle est codifiée avec moins bits (seulement 2)
	qui une lettre moins fréquente (comme la lettre "G", avec 10 bits).
  */

/* Question 4 : calcul du taux de compression : 
le passage suivant est à décommenter après la question 3 :
*/
  lc = nb_bits(arbre, 0);
  printf("longueur (en bits) du texte compresse = %d Sans compression=%d\n", lc, l*8);
  printf("Taxe de compr: %.4f\n", (float) (l*8)/lc );
  /*
	Question 4
	La taux de compression pour "Huffman.txt" est plus petit que la taux du "test.txt".
	Cela ocurrs parce que la compression pour Huffman est déjà la plus optimal (avec ASCII 8 bits).
	Par contre, le texte plus petit peut être optimizé  avec l'usage de moins bits.
  */

// Question 5 calculer la longueur moyenne du code obtenu :
    printf("Longueur moyenne: %.3f.\n", nb_moyen_bits(arbre, 0));
    
    /* Question 5
        L'égalité entre entropie et longueur moyenne dans le fichier "test.txt" s'est produit
        pour sa haute efficacité (égale à 1, car E = H / l). C'est-à-dire que la longueur de
        3 bits est idéale pour ce texte. Par contre, le texte "Huffman.txt"
        présente une eficacité un peu plus basse (0.993). C'est-à-dire que cette codage
        est moins efficace (il y a plus bits que c'était nécéssaire).
    */
return 1 ;
}
