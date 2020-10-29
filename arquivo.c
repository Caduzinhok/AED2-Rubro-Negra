#include <stdio.h>
#include <stdlib.h>
//Definição para Arvore Rubro Negra
#define RED 1
#define BLACK 0
//Struct Base para utilização
typedef struct aux {
    int chave;
    struct aux * esq;
    struct aux * dir;
    int cor;
}NO;
//Funções para implementação
NO* move2DirRED(NO* no);
NO* start();
NO*busca(int chave, NO* raiz);
void inverte_cor(NO* no);
NO* rotacao_simples_dir(NO* A);
NO* rotacao_simples_esq(NO* A);
NO* move2EsqRED(NO* no);
NO* move2DirRED(NO* no);
NO* balancear(NO* no);
NO* insereNO(NO* no,int valor, int *resp);
int removendo(NO* raiz, int valor);
NO* removendo_NO(NO* no, int valor);
NO* removerMenor(NO* no);
NO* procuraMenor(NO* atual);
int consulta(NO *no, int valor);

NO* start() {
    return NULL;
}

int cor(NO *no){
    if(no == NULL){
        return BLACK;
    }
    return no->cor;
}

void inverte_cor(NO* no){
    //Troca a cor do nó
    no->cor = !no->cor;
    if(no->esq != NULL){
        //Troca a cor das SubArvore á esquerda
        no->esq->cor = !no->esq->cor;
    }
    if(no->dir != NULL){
        //Troca a cor da SubArvore á direita
        no->dir->cor = !no->dir->cor;
    }
}

NO* rotacao_simples_dir(NO* A){
    /*Rotação simples a direita como em uma Arvore AVL, porém seguindo o sistema de cores da árvore*/
    NO* B = A->esq;
    A->esq = B->dir;
    B->dir = A;
    B->cor = A->cor;
    A->cor = RED;
    return B;
}

NO* rotacao_simples_esq(NO* A){
    /*Rotação simples a esquerda como em uma Arvore AVL, porém seguindo o sistema de cores da árvore*/
    NO* B = A->dir;
    A->dir = B->esq;
    B->esq = A;
    B->cor = A->cor;
    A->cor = RED;
    return B;
}

NO* move2EsqRED(NO* no){
    inverte_cor(no);
    if((cor(no->dir->esq)) == RED){
        no->dir = rotacao_simples_dir(no->dir);
        no = rotacao_simples_esq(no);
        inverte_cor(no);
    }
    return no;
}

NO* move2DirRED(NO* no){
    inverte_cor(no);
    if((cor(no->esq->esq)) == RED){
        no = rotacao_simples_dir(no);
        inverte_cor(no);
    }
    return no;
}

NO* balancear(NO* no){
    //nó vermelho é sempre filho à esquerda
    if(cor(no->dir) == RED){
        no = rotacao_simples_dir(no);
    }
    //Filho da direita e neto da esquerda são vermelho
    if(no->esq != NULL && cor(no->dir) == RED && cor(no->esq->esq) == RED){
        no = rotacao_simples_dir(no);
    //2 filhos vermelhos: troca a cor
    }
    if(cor(no->esq) == RED && cor(no->dir) == RED){
        inverte_cor(no);
    }
    return no;
}

int inserir(NO **raiz, int valor){
    int resp;
    //FUNÇÃO RESPONSÁVEL PELA BUSCA DO LOCAL DE INSERÇÃO DO NÓ
    *raiz = insereNO(*raiz,valor,&resp);
    if(raiz != NULL){
        (*raiz)->cor = BLACK;
    }
    return resp;
}

NO* insereNO(NO* no,int valor, int *resp){
    if(!no){
        NO *novo = (NO*)malloc(sizeof(NO));
        if(!novo){
            *resp = 0;
            return NULL;
        }
        novo->chave = valor;
        novo->cor = RED;
        novo->dir = NULL;
        novo->esq = NULL;
        *resp = 1;
        return novo;
    }
    if(valor == no->chave){
        *resp = 0;
    }
    else{
        if(valor < no->chave){
            no->esq = insereNO(no->esq, valor, resp);
        }
        else{
            no->dir = insereNO(no->dir, valor, resp);
        }
    }
    if(cor(no->dir) == RED && cor(no->esq) == BLACK){
        no = rotacao_simples_esq(no);
    }
    if(cor(no->esq) == RED && cor(no->esq->esq) == RED){
        no = rotacao_simples_dir(no);
    }
    if(cor(no->esq) == RED && cor(no->dir) == RED){
        inverte_cor(no);
    }
    return no;
}

int removendo(NO* raiz, int valor){
    if(consulta(raiz, valor)){
        NO* no = raiz;
        //Função responsável pela busca do nó a ser removido
        raiz = removendo_NO(no, valor);
        if(raiz != NULL){
            //Deixa a raiz sempre como preto.
            raiz->cor = BLACK;
        }
        return 1;
    }else{
        return 0;
    }
}

NO* removendo_NO(NO* no, int valor){
    //Verifica se o valor é menor que o chave verificada.
    if(valor < no->chave){
        if(cor(no->esq) == BLACK && cor(no->esq->esq) == BLACK){
            /*Se o filho do no a esquerda é preto e o neto a esquerda tambem é preto e faz uma rotação a direita e outra para a esquerda redistribuindo e organizando a arvore*/
            no = move2EsqRED(no);
        }
    /*Caso o valor seja menor, chama recursivamente para continuar verificando até achar o nó, passando o nó menor a esquerda*/
    no->esq = removendo_NO(no->esq, valor);
    }
    /*Caso o valor seja maior que a chave*/
    else{
        //Verifica se a cor do nó a esquerda é vermelho
        if(cor(no->esq) == RED){
            //Se for vermelha rotaciona a direita
            no = rotacao_simples_dir(no);
        }
        if(valor == no->chave && (no->dir == NULL)){
            //Se encontrar o valor e a SubArvore da direita não existir, então apenas apaga aquele valor.
            free(no);
            return NULL;
        }
        if(cor(no->dir) == BLACK && cor(no->dir->esq) == BLACK){
            /*Caso a SubArvore a direita seja preta e neto á esquerda da SubArvore da direita também, realiza uma rotação direita e inverte a cor desse nó*/
            no = move2DirRED(no);
        }
        if(valor == no->chave){
            /*Encontrando o valor, faz uma substituição nos valores, assumindo um x auxiliar e buscando o menor valor da SubArvore a direita para substituir e a remove logo após*/
            NO* x = procuraMenor(no->dir);
            no->chave = x->chave;
            no->dir = removerMenor(no->dir);
        }else{
            /*Caso não encontre o valor, ele chama a recursão novamente passando o proximo valor da SubArvore a direita*/
            no->dir = removendo_NO(no->dir, valor);
        }
    }
    /*Por fim, balanceia o nó para descobrir possíveis falhas e desbalanceamentos das cores da arvore e valores*/
    return balancear(no);
}

NO* removerMenor(NO* no){
    if(no->esq == NULL){
        free(no);
        return NULL;
    }
    if(cor(no->esq) == BLACK && cor(no->esq->esq) == BLACK){
        no = move2EsqRED(no);
    }
    no->esq = removerMenor(no->esq);
    return balancear(no);
}

NO* procuraMenor(NO* atual){
    NO* no1 = atual;
    NO* no2 =  atual->esq;
    while(no2 != NULL){
        no1 = no2;
        no2 = no2->esq;
    }
    return no1;
}

int consulta(NO *no, int valor){
    NO* aux = no;
    while(aux){
        if(valor == aux->chave){
            return 1;
        }
        if(valor > aux->chave){
            aux = aux->dir;
        }
        else{
            aux = aux->esq;
        }
    }
    return 'NULL';
}

void mostrar(NO* no){
    //Função para impressão de elementos recursivamente

    printf("(");
    if(no){
        if(no->cor == RED){
            printf("%d ",no->chave);
        }
        else{
            printf("%d ",no->chave);
        }
        //SubArvore á esquerda
        mostrar(no->esq);
        //SubArvore á direita
        mostrar(no->dir);
    }
    printf(")");
}
int main(void) {
    NO* no = start();
    int j = 8, X,
    dadosTeste[10] = {10, 13, 8, 7, 4, 15, 11, 18, 6, 1};
    int dadosMaiores[27] = {1,30,29,2,3,28,27,4,5,26,25,23,24,6,8,7,23,22,20,15,21,9,11,10,19,17,16};
    while(j){
        inserir(&no,dadosTeste[j]);
        j--;
    }
    mostrar(no);
    printf("\n\n");
    removendo(no,15);
    printf("\n\n");
    removendo(no, 8);
    mostrar(no);
    printf("\n\n");
}
