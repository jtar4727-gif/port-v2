/* Modos jogaveis do 3DS: exploracao 2D da casa, minigame do balcao, apertar botao e QTE.
   Incluido por main.c depois das funcoes de desenho basicas. */
#ifndef EXPLORE_H
#define EXPLORE_H
#include "script.h"

/* ---- acoes (dialogos curtos que voltam para a exploracao) ---- */
static const Node A_GAVETA_TRANC[] = { NAR("A gaveta está trancada. A fechadura é pequena, precisa de uma chavinha."), END() };
static const Node A_GAVETA_OK[]    = { NAR("A chavinha gira. Dentro da gaveta, luvas de borracha e um rolo de fita adesiva."), END() };
static const Node A_BANCADA[]      = { NAR("A gaveta da bancada está emperrada."), MASH("ABRIR A GAVETA !", 8), NAR("LUCAS força a gaveta até ela ceder. Lá dentro, um serrote enferrujado."), END() };
static const Node A_QUADRO[]       = { NAR("Entre várias chaves penduradas, uma com etiqueta escrita à mão: “QUARTO”."), END() };
static const Node A_ESCRIV[]       = { NAR("Embaixo de um caderno de capa preta, uma chave grande e pesada: a chave do porão. No caderno, a letra do MATEUS: “ninguém pode saber”."), END() };
static const Node A_SOFA[]         = { NAR("Entre as almofadas do sofá, LUCAS encontra uma chavinha pequena, daquelas de gaveta."), END() };
static const Node A_QUARTO_TRANC[] = { NAR("A porta lá em cima está trancada."), END() };
static const Node A_PORAO_TRANC[]  = { NAR("A porta do porão está trancada. Faltam o serrote, as luvas e a chave."), END() };
static const Node A_PORAO_OK[]     = { NAR("A chave gira devagar. A porta do porão se abre sozinha, soltando um cheiro forte de produto químico."), END() };
static const Node A_TV[]           = { ART(ART_BAILE), NAR("LUCAS liga a TV. Passa a reprise de um filme adolescente: um baile escolar, luzes roxas e casais dançando devagar, ao som de uma balada em 16 bits."),
  SAY("LUCAS (rindo baixo)", "Nossa… que clichê. Purple Rain em um baile escolar, com tudo roxo."), NAR("Por um instante, o barulho da casa some. Ele quase sorri. Depois a TV chia e ele desliga."), ART(ART_NONE), END() };

enum { I_SERROTE = 1, I_LUVAS = 2, I_FITA = 4, I_CQ = 8, I_CG = 16, I_CP = 32 };
enum { H_TV = 1, H_SOFA, H_ESCADA, H_PORAO, H_GAVETA, H_BANCADA, H_QUADRO, H_ESCRIV };
typedef struct { int room; float x, rad; const char *lbl; int id; } Hot;
/* salas: 1 sala, 3 garagem, 4 cozinha, 6 quarto  (x em 0..160, a faixa do chao e y 82..114) */
static const Hot HOTS[] = {
  { 1, 62, 18, "Ver a televisão", H_TV }, { 1, 70, 20, "Procurar no sofá", H_SOFA }, { 1, 138, 14, "Subir a escada", H_ESCADA }, { 1, 100, 14, "Porta do porão", H_PORAO },
  { 4, 70, 22, "Abrir a gaveta", H_GAVETA }, { 3, 100, 22, "Revirar a bancada", H_BANCADA }, { 3, 36, 20, "Quadro de chaves", H_QUADRO }, { 6, 40, 22, "Revirar a escrivaninha", H_ESCRIV },
};
static int exRoom, inv, tvDone;
static float px, py;
static const char *exMsg; static float exMsgT;

static void enter(void);
static void pushNodes(const Node *a) { if (!pushed) { retChap = chap; retNi = ni; } pushed = 1; chap = a; ni = 0; enter(); }
static void exInit(void) { exRoom = 1; inv = 0; tvDone = 0; px = 80; py = 100; pushed = 0; exploreDone = 0; locId = 1; }
static int hotVisible(const Hot *h) {
  switch (h->id) {
    case H_TV: return !tvDone;
    case H_SOFA: return !(inv & I_CG);
    case H_GAVETA: return !(inv & I_LUVAS);
    case H_BANCADA: return !(inv & I_SERROTE);
    case H_QUADRO: return !(inv & I_CQ);
    case H_ESCRIV: return !(inv & I_CP);
  }
  return 1;
}
static const Hot *nearHot(void) {
  for (unsigned i = 0; i < sizeof HOTS / sizeof *HOTS; i++) { const Hot *h = &HOTS[i]; if (h->room == exRoom && hotVisible(h) && fabsf(px - h->x) < h->rad) return h; }
  return 0;
}
static void doHot(const Hot *h) {
  switch (h->id) {
    case H_TV: tvDone = 1; pushNodes(A_TV); break;
    case H_SOFA: inv |= I_CG; pushNodes(A_SOFA); break;
    case H_GAVETA: if (inv & I_CG) { inv |= I_LUVAS | I_FITA; pushNodes(A_GAVETA_OK); } else pushNodes(A_GAVETA_TRANC); break;
    case H_BANCADA: inv |= I_SERROTE; pushNodes(A_BANCADA); break;
    case H_QUADRO: inv |= I_CQ; pushNodes(A_QUADRO); break;
    case H_ESCRIV: inv |= I_CP; pushNodes(A_ESCRIV); break;
    case H_ESCADA: if (inv & I_CQ) { exRoom = 6; locId = 6; px = 14; } else pushNodes(A_QUARTO_TRANC); break;
    case H_PORAO: if ((inv & I_CP) && (inv & I_SERROTE) && (inv & I_LUVAS)) { exploreDone = 1; pushNodes(A_PORAO_OK); } else pushNodes(A_PORAO_TRANC); break;
  }
}
static void updatePlay(float dt, u32 kd, u32 kh) {
  (void)kh; clockT += dt;
  circlePosition cp; hidCircleRead(&cp);
  float vx = cp.dx / 156.0f, vy = -cp.dy / 156.0f;
  if (kh & KEY_LEFT) vx = -1; if (kh & KEY_RIGHT) vx = 1; if (kh & KEY_UP) vy = -1; if (kh & KEY_DOWN) vy = 1;
  px += vx * 60 * dt; py += vy * 40 * dt;
  if (py < 84) py = 84; if (py > 114) py = 114;
  /* portas nas bordas */
  if (px < 3) {
    if (exRoom == 1) { exRoom = 3; px = 150; } else if (exRoom == 4 || exRoom == 6) { exRoom = 1; px = (exRoom == 1 && locId == 6) ? 128 : 150; }
    else px = 3;
  } else if (px > 157) {
    if (exRoom == 1) { exRoom = 4; px = 8; } else if (exRoom == 3) { exRoom = 1; px = 10; } else px = 157;
  }
  locId = exRoom;
  if (kd & KEY_A) { const Hot *h = nearHot(); if (h) doHot(h); }
  if (kd & KEY_SELECT) toMenu();
}
static void drawPlayTop(void) {
  C2D_DrawRectSolid(0, 0, 0, 400, 240, HEX(0x000000)); drawRoom(exRoom);
  for (unsigned i = 0; i < sizeof HOTS / sizeof *HOTS; i++) { const Hot *h = &HOTS[i]; if (h->room == exRoom && hotVisible(h)) { float b = sinf(clockT * 5) * 1.5f; R(h->x - 2, 74 + b, 4, 4, 0xe8c040); } }
  if (exRoom == 1) { R(150, 78, 6, 38, 0x2a1c14); R(0, 78, 5, 38, 0x2a1c14); } else { R(0, 78, 5, 38, 0x2a1c14); R(155, 78, 5, 38, 0x2a1c14); }
  person(px, py + 4, 1.0f, 0, 0x6a7a8a, 0x332211);
  C2D_DrawRectSolid(0, 0, 0, 40, 240, HEX(0x000000)); C2D_DrawRectSolid(360, 0, 0, 40, 240, HEX(0x000000));
}
static void drawPlayBottom(void) {
  C2D_DrawRectSolid(0, 0, 0, 320, 240, HEX(0x0c0907));
  txt("Procure as ferramentas e a chave do porão", 12, 10, 0.5f, HEX(0xc2ae6a), 296, 0);
  static const char *nm[6] = { "Serrote", "Luvas", "Fita", "Chave do quarto", "Chave pequena", "Chave do porão" };
  txt("Itens:", 12, 56, 0.5f, HEX(0x8a8170), 0, 0);
  int y = 76;
  for (int i = 0; i < 6; i++) if (inv & (1 << i)) { txt(nm[i], 20, y, 0.5f, HEX(0xd8ceb4), 0, 0); y += 18; }
  if (!inv) txt("nenhum", 20, 76, 0.5f, HEX(0x6b6352), 0, 0);
  const Hot *h = nearHot();
  if (h) { C2D_DrawRectSolid(8, 196, 0, 304, 30, HEX(0x2e2214)); char b[80]; snprintf(b, sizeof b, "A: %s", h->lbl); txt(b, 16, 202, 0.6f, HEX(0xffffff), 0, 0); }
  else txt("Circle Pad / direcional: andar     SELECT: menu", 12, 222, 0.4f, HEX(0x6b6352), 0, 0);
}

/* ---- minigame do balcao ---- */
static const char *SV_IT[6] = { "Hambúrguer", "Cheddar", "Batata frita", "Refrigerante", "Sorvete", "Nuggets" };
static const char *SV_NA[8] = { "Dona Marta", "Seu Jorge", "Tiago", "Camila", "Um senhor apressado", "Uma criança", "Rapaz de fone", "Dona Neusa" };
static int svSel, svOrder, svCust, svServed;
static float svT;
static void svNew(void) { int n = 2 + rand() % 2; svOrder = 0; while (__builtin_popcount(svOrder) < n) svOrder |= 1 << (rand() % 6); svSel = 0; svCust = rand() % 8; svT = 16.0f; }
static void svInit(void) { svServed = 0; svNew(); }
static void svDeliver(void) { if (svSel == svOrder) svServed++; else shakeT = 0.3f; svNew(); }
static void updateServe(float dt, u32 kd, touchPosition tp) {
  clockT += dt; svT -= dt; if (svT <= 0) { shakeT = 0.3f; svNew(); }
  if (shakeT > 0) shakeT -= dt;
  if (kd & KEY_TOUCH) {
    for (int i = 0; i < 6; i++) { int bx = 10 + (i % 2) * 154, by = 6 + (i / 2) * 42; if (tp.px >= bx && tp.px < bx + 146 && tp.py >= by && tp.py < by + 38) svSel ^= 1 << i; }
    if (tp.py >= 136 && tp.py < 180) svDeliver();
    else if (tp.py >= 188 && tp.py < 232 && svServed >= 3) { advance(); return; }
  }
  if (kd & KEY_A) svDeliver();
  if ((kd & KEY_X) && svServed >= 3) advance();
}
static void drawServeTop(void) {
  C2D_DrawRectSolid(0, 0, 0, 400, 240, HEX(0x000000)); drawRoom(8);
  R(0, 78, 160, 8, 0x8a8478); person(60, 80, 1.4f, 0, 0x7a8a6a, 0x332211);
  C2D_DrawRectSolid(60, 6, 0, 280, 90, HEX(0x16110d));
  char b[160]; snprintf(b, sizeof b, "%s pede:", SV_NA[svCust]); txt(b, 70, 10, 0.6f, HEX(0xd8ceb4), 0, 0);
  b[0] = 0; for (int i = 0; i < 6; i++) if (svOrder & (1 << i)) { strcat(b, SV_IT[i]); strcat(b, "   "); }
  txt(b, 70, 36, 0.6f, HEX(0xffe080), 260, 0);
  snprintf(b, sizeof b, "Atendidos: %d%s", svServed, svServed >= 3 ? "  (pode sair)" : " / 3"); txt(b, 70, 74, 0.5f, HEX(0x8a8170), 0, 0);
  C2D_DrawRectSolid(60, 98, 0, 280 * (svT / 16.0f), 6, HEX(0xa8301f));
}
static void drawServeBottom(void) {
  C2D_DrawRectSolid(0, 0, 0, 320, 240, HEX(0x0c0907));
  for (int i = 0; i < 6; i++) { int bx = 10 + (i % 2) * 154, by = 6 + (i / 2) * 42; C2D_DrawRectSolid(bx, by, 0, 146, 38, (svSel >> i) & 1 ? HEX(0x3a5a2a) : HEX(0x241a10)); txt(SV_IT[i], bx + 8, by + 8, 0.55f, HEX(0xe6dcc4), 0, 0); }
  C2D_DrawRectSolid(10, 136, 0, 300, 44, HEX(0x5a3a14)); txt("ENTREGAR   (A)", 90, 148, 0.7f, HEX(0xffffff), 0, 0);
  if (svServed >= 3) { C2D_DrawRectSolid(10, 188, 0, 300, 44, HEX(0x4a1a1a)); txt("Ir para a sala dos fundos  (X)", 40, 202, 0.55f, HEX(0xffffff), 0, 0); }
}

/* ---- apertar botao (mash) e QTE ---- */
static int mhCnt, mhNeed, qtKey; static float mhT, qtT; static const char *mhLabel;
static const char *QK[4] = { "A", "B", "X", "Y" };
static const u32 QKM[4] = { KEY_A, KEY_B, KEY_X, KEY_Y };
static void mashInit(const Node *n) { mhLabel = n->a; mhNeed = n->n ? n->n : 8; mhCnt = 0; mhT = 4.5f; }
static void qteInit(const Node *n) { mhLabel = n->a; qtKey = rand() % 4; qtT = 3.0f; qtFail = -1; qtFailCh = -1; qtOk = -1; }
static void updateMash(float dt, u32 kd) { clockT += dt; mhT -= dt; if (mhT <= 0) { mhCnt = 0; mhT = 4.5f; } if (kd & KEY_A) { if (++mhCnt >= mhNeed) advance(); } }
static void qteFailed(void) {
  if (qtFail >= 0) { ni = qtFail; enter(); return; }
  if (qtFailCh >= 0) { gotoChapter(qtFailCh); return; }
  qtKey = rand() % 4; qtT = 3.0f; shakeT = 0.3f;
}
static void updateQte(float dt, u32 kd) {
  clockT += dt; qtT -= dt;
  if (shakeT > 0) shakeT -= dt;
  if (qtT <= 0) { qteFailed(); return; }
  if (kd & (KEY_A | KEY_B | KEY_X | KEY_Y)) { if (kd & QKM[qtKey]) { if (qtOk >= 0) gotoChapter(qtOk); else advance(); } else qteFailed(); }
}
static void drawMiniTop(int isQte) {
  C2D_DrawRectSolid(0, 0, 0, 400, 240, HEX(0x000000));
  if (locId) drawRoom(locId);
  C2D_DrawRectSolid(0, 0, 0, 400, 240, C2D_Color32(0, 0, 0, 150));
  txt(mhLabel, 200, 40, 0.9f, HEX(0xffe080), 0, C2D_AlignCenter);
  if (isQte) { txt(QK[qtKey], 200, 90, 2.4f, HEX(0xffffff), 0, C2D_AlignCenter); C2D_DrawRectSolid(60, 200, 0, 280 * (qtT / 3.0f), 8, HEX(0xa8301f)); }
  else { C2D_DrawRectSolid(60, 120, 0, 280, 16, HEX(0x241a10)); C2D_DrawRectSolid(60, 120, 0, 280.0f * mhCnt / mhNeed, 16, HEX(0xc2ae6a)); C2D_DrawRectSolid(60, 200, 0, 280 * (mhT / 4.5f), 6, HEX(0xa8301f)); }
}
static void drawMiniBottom(int isQte) {
  C2D_DrawRectSolid(0, 0, 0, 320, 240, HEX(0x0c0907));
  txt(isQte ? "Aperte o botão mostrado na tela de cima !" : "Aperte A várias vezes !", 160, 100, 0.6f, HEX(0xd8ceb4), 0, C2D_AlignCenter);
}
#endif
