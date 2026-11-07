/* Roteiro: "Quero Acabar Logo Com Isso" (Arthur Abreu) — dados do jogo.
   Cada capitulo e um vetor de nos. Escolhas: a 1a opcao soma "ternura", a 2a soma "raiva". */
#ifndef SCRIPT_H
#define SCRIPT_H

typedef enum { N_SAY, N_NAR, N_CARD, N_CHOICE, N_ART, N_MUS, N_FX, N_END, N_SERVE, N_MASH, N_QTE, N_PLAY, N_SETF, N_IFJ, N_IFN, N_JMP, N_CHAP, N_QTEF, N_QTES, N_QTEFC, N_CHP, N_IFPTS, N_CHCH } NodeType;
typedef struct { NodeType t; const char *a, *b, *c; int n, m; } Node;

enum { ART_NONE, ART_PATEO, ART_MACA, ART_PRETO, ART_BAILE };
enum { MUS_OFF, MUS_CALM, MUS_CHAOS, MUS_TENSE };
enum { FX_SHAKE = 1, FX_FLASH };

#define SAY(who, txt)       { N_SAY, who, txt, 0, 0 }
#define NAR(txt)            { N_NAR, txt, 0, 0, 0 }
#define CARD(title, sub, s) { N_CARD, title, sub, 0, s }   /* s = segundos (0 = espera botao A) */
#define ART(id)             { N_ART, 0, 0, 0, id }
#define MUS(id)             { N_MUS, 0, 0, 0, id }
#define FX(id)              { N_FX, 0, 0, 0, id }
#define CHOICE(q, o1, o2)   { N_CHOICE, q, o1, o2, 0 }
#define SERVE()             { N_SERVE, 0, 0, 0, 0 }
#define MASH(lbl, k)        { N_MASH, lbl, 0, 0, k }
#define QTE(lbl)            { N_QTE, lbl, 0, 0, 0 }
#define PLAY(id)            { N_PLAY, 0, 0, 0, id }
#define SETF(i, v)          { N_SETF, 0, 0, 0, i, v }
#define IFJ(f, t)           { N_IFJ, 0, 0, 0, t, f }
#define IFN(f, t)           { N_IFN, 0, 0, 0, t, f }
#define JMP(t)              { N_JMP, 0, 0, 0, t, 0 }
#define CHAP(c)             { N_CHAP, 0, 0, 0, c, 0 }
#define QTEF(lbl, t)        { N_QTEF, lbl, 0, 0, t, 0 }
#define QTES(lbl, ch, mk)   { N_QTES, lbl, 0, 0, ch, mk }
#define QTEFC(lbl, ch)      { N_QTEFC, lbl, 0, 0, ch, 0 }
#define CHP(q, ok, bad)     { N_CHP, q, ok, bad, 0, 0 }
#define IFPTS(mn, t)        { N_IFPTS, 0, 0, 0, t, mn }
#define CHCH(q, o1, o2, ch) { N_CHCH, q, o1, o2, ch, 0 }
#define END()               { N_END, 0, 0, 0, 0 }

static const Node CH0[] = {
  MUS(MUS_CALM),
  CARD("Ext. Dia - Pátio da escola", "", 2),
  ART(ART_PATEO),
  NAR("No recreio de uma turma do quinto ano, vemos LUCAS em uma cadeira no canto do pátio, vendo as outras crianças se divertindo no intervalo, até que…"),
  SAY("MATEUS CRIANÇA", "Você consegue descascar pra mim ?"),
  ART(ART_MACA),
  CHOICE("Mateus estende uma maçã e uma faca pequena.", "Aceitar, em silêncio", "Aceitar, mas encarar ele um pouco"),
  SAY("LUCAS CRIANÇA", "Claro…"),
  NAR("MATEUS se senta ao lado de LUCAS, que começa a descascar a maçã com uma faca pequena."),
  SAY("MATEUS CRIANÇA", "Como você consegue ficar assim ?"),
  SAY("LUCAS CRIANÇA", "Assim como ?"),
  SAY("MATEUS CRIANÇA", "Assim no canto… sem ninguém… igual a um esquisito"),
  SAY("LUCAS CRIANÇA", "É que eu to a uns dias tentando desenhar o pátio"),
  ART(ART_PATEO),
  NAR("Nesse momento, LUCAS pega um caderno que estava do seu lado e mostra para MATEUS um desenho surpreendentemente bem feito, porém incompleto, do pátio da escola."),
  SAY("MATEUS CRIANÇA", "Eh… legal eu acho… você acha que tem talento pra essas coisas ?"),
  SAY("LUCAS CRIANÇA", "Tem vezes que eu acho que sim, é o que eu quero ser quando eu crescer, pintor ou desenhista tanto faz"),
  SAY("MATEUS CRIANÇA", "Você sabe que isso não dá dinheiro né ?"),
  SAY("LUCAS CRIANÇA", "Se eu for bom no que eu faço, não vai ter como dar errado né ?"),
  SAY("MATEUS CRIANÇA", "Quem me dera se as coisas fosse tão simples assim"),
  SAY("LUCAS CRIANÇA", "Beleza então, o que você quer ser quando crescer ?"),
  SAY("MATEUS CRIANÇA", "Policial, com certeza"),
  SAY("LUCAS CRIANÇA", "Porque ?"),
  SAY("MATEUS CRIANÇA", "Sei lá, prender todos os bandidos que eu quiser, sair com um carrão por aí e andar armado, essas coisas…"),
  ART(ART_MACA),
  NAR("LUCAS tem dificuldades para tirar a casca da maçã."),
  SAY("LUCAS CRIANÇA (raiva)", "Droga ! Isso ta uma merda de tirar"),
  SAY("MATEUS CRIANÇA (impaciente)", "Me dá isso !"),
  NAR("MATEUS pega a maçã da mão de LUCAS."),
  SAY("MATEUS CRIANÇA", "Você não consegue tirar a casca de uma maçã ? É sério isso ?"),
  SAY("LUCAS CRIANÇA", "Desculpa, é que eu não ( interrompido )"),
  SAY("MATEUS CRIANÇA", "Não importa, deixa que eu tiro isso logo de uma vez"),
  SAY("MATEUS CRIANÇA (raiva)", "Tá vendo ? E olha que eu não tava conseguindo, você precisa saber cortar as coisas, sabe ? Não é força é jeito"),
  MUS(MUS_CHAOS), FX(FX_SHAKE), FX(FX_FLASH),
  NAR("MATEUS corta a maçã cada vez mais rápido, forte, visceral e doentio. O suco escorre da maçã como se fosse sangue."),
  SAY("LUCAS CRIANÇA", "Ei ! Vai com calma, pra que tanta pressa assim ?"),
  NAR("MATEUS para de cortar a maçã, respira fundo e diz:"),
  SAY("MATEUS CRIANÇA", "É que eu…"),
  ART(ART_PRETO), MUS(MUS_OFF),
  CARD("QUERO ACABAR LOGO COM ISSO", "( CRÉDITOS INICIAIS )  música: Enslaved by Machinery", 5),
  END()
};

#endif
