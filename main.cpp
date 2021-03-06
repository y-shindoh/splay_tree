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
	ys::SplayTree<unsigned int, unsigned int> st;

	for (int i(0); i < M; ++i) st.add(values[i], i);
	st.print();

	for (int i(0); i < N; ++i) {
		int j = st.find(keys[i]);
		if (j != decltype(st)::InvalidValue()) {
			std::printf(">>>> %u: FOUND (%u)\n", keys[i], j);
		}
		else {
			std::printf(">>>> %u: NOT FOUND\n", keys[i]);
		}
		st.print();
	}

	std::printf(">>>>\n");
	st.remove(8);
	st.print();

	for (int i(0); i < N; ++i) {
		int j = st.find(keys[i]);
		if (j != decltype(st)::InvalidValue()) {
			std::printf(">>>> %u: FOUND (%u)\n", keys[i], j);
		}
		else {
			std::printf(">>>> %u: NOT FOUND\n", keys[i]);
		}
		st.print();
	}

	return 0;
}
