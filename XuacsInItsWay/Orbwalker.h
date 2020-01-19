


#ifndef ORBWALKER_H

#define ORBWALKER_H

static class Orbwalker {
public:
	static void Update();
	static void LaneClear();
	static void LastHit();
	static void Combo();


	static int harras_key;
	static int combo_key;
	static int last_hit_key;

	static float adjust_for_attack_cast_delay;
	static float adjust_for_attack_delay;
};


#endif // !ORBWALKER_H


