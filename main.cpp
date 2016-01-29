/* -*- coding: utf-8; tab-width: 4 -*- */
/**
 * @file	main.cpp
 * @brief	kd_search_array.hppの動作確認用コマンド
 * @author	Yasutaka SHINDOH / 新堂 安孝
 */

#include <cstdio>
#include "splay_tree.hpp"

#define	M	15
#define	N	5

/**
 * サンプル・コマンド
 */
int
main()
{
	int values[M] = {7, 3, 11, 1, 5, 9, 13, 0, 2, 4, 6, 8, 10, 12, 14};
	int keys[N] = {2, 0, 8, 13, 5};
	ys::Node<int, int>* st(0);
	ys::Node<int, int>* n;

	for (int i(0); i < M; ++i) {
		st = ys::Node<int, int>::Add(st, values[i], i);
	}

	ys::Node<int, int>::Print(st);

	for (int i(0); i < N; ++i) {
		n = ys::Node<int, int>::Find(st, keys[i]);
		if (n) {
			std::printf(">>>> %d: FOUND (%d)\n", keys[i], n->v_);
		}
		else {
			std::printf(">>>> %d: NOT FOUND\n", keys[i]);
		}
		ys::Node<int, int>::Print(st);
	}

	std::printf(">>>>\n");
	st = ys::Node<int, int>::DeleteRoot(st);
	ys::Node<int, int>::Print(st);

	for (int i(0); i < N; ++i) {
		n = ys::Node<int, int>::Find(st, keys[i]);
		if (n) {
			std::printf(">>>> %d: FOUND (%d)\n", keys[i], n->v_);
		}
		else {
			std::printf(">>>> %d: NOT FOUND\n", keys[i]);
		}
		ys::Node<int, int>::Print(st);
	}

	ys::Node<int, int>::DeleteAll(st);

	return 0;
}
