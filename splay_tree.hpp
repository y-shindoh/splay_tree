/* -*- coding: utf-8; tab-width: 4 -*- */
/**
 * @file	splay_tree.hpp
 * @brief	Splay Tree / スプレー木
 * @author	Yasutaka SHINDOH / 新堂 安孝
 * @note	see https://ja.wikipedia.org/wiki/%E3%82%B9%E3%83%97%E3%83%AC%E3%83%BC%E6%9C%A8 .
 */

#ifndef	__SPLAY_TREE_HPP__
#define	__SPLAY_TREE_HPP__	"splay_tree.hpp"

#include <cstdio>
#include <cassert>
#include <utility>

namespace ys
{
	/**
	 * スプレー木のノード
	 */
	template<typename K_, typename V_>
	class Node
	{
		/**
		 * コンストラクタ
		 * @param[in]	k	キー
		 */
		Node(const K_& k,
			 const V_& v)
			: k_(k), v_(v)
			{
				c_[0] =  c_[1] = 0;
			}

		/**
		 * デストラクタ
		 */
		~Node() = default;

		/**
		 * キーの探索 (サブ・ルーチン)
		 * @param[in,out]	n	処理対象ノード
		 * @param[in,out]	p	引数 @a n の親ノード
		 * @param[in]	k	キー
		 * @param[in]	i	親子のキーの関係 (0: 親が大きい, 1: 同じか子が大きい)
		 * @param[out]	r	キー @a k を持つノードの有無
		 * @return	探索後の処理対象ノード
		 * @note	簡単にするためzig-zigステップは実装していない。
		 */
		static Node<K_, V_>*
		Find(Node<K_, V_>* n,
			 Node<K_, V_>*& p,
			 const K_& k,
			 int i,
			 bool& r)
			{
				Node<K_, V_>* c(0);

				if (n) {
					if (k != n->k_) {
						int j = (int)(k > n->k_);
						c = Find(n->c_[j], n, k, j, r);
						if (!r) return n;	// not found
						n->c_[1-j] = c;
					}
					else {
						r = true;
					}

					c = p;
					p->c_[i] = n->c_[1-i];
					p = n;
				}

				return c;
			}

	public:

		Node<K_, V_>* c_[2];	///< 子ノード
		K_ k_;	///< キー
		V_ v_;	///< 値

		/**
		 * スプレー木にノードを追加
		 * @param[in,out]	n	スプレー木のルート・ノード
		 * @param[in]	k	追加対象のキー
		 * @return	追加処理後のルート・ノード
		 */
		static Node<K_, V_>*
		Add(Node<K_, V_>* n,
			const K_& k,
			const V_& v)
			{
				if (n) {
					int i = (int)(n->k_ <= k);
					n->c_[i] = Add(n->c_[i], k, v);
				}
				else {
					n = new Node<K_, V_>(k, v);
				}

				return n;
			}

		/**
		 * スプレー木のルート・ノードを削除
		 * @param[in,out]	n	スプレー木のルート・ノード
		 * @return	削除処理後のルート・ノード
		 */
		static Node<K_, V_>*
		DeleteRoot(Node<K_, V_>* n)
			{
				if (!n) return 0;

				Node<K_, V_>* c;

				if (n->c_[1]) {
					Node<K_, V_>* p(0);
					c = n->c_[1];

					while (c->c_[0]) {
						p = c;
						c = c->c_[0];
					}

					if (p) {
						p->c_[0] = c->c_[1];
						c->c_[1] = n->c_[1];
					}
					c->c_[0] = n->c_[0];
				}
				else {
					c = n->c_[0];
				}

				delete n;
				return c;
			}

		/**
		 * スプレー木の全てのノードを削除
		 * @param[in,out]	n	スプレー木のルート・ノード
		 */
		static void
		DeleteAll(Node<K_, V_>* n)
			{
				if (n) {
					for (size_t i(0); i < 2; ++i) {
						if (n->c_[i]) DeleteAll(n->c_[i]);
					}

					delete n;
				}
			}

		/**
		 * スプレー木内のキーの探索
		 * @param[in,out]	n	スプレー木のルート・ノード
		 * @param[in]	k	キー
		 * @return	キー @a k を持つノードのうち最初のもの
		 * @note	探索失敗時は 0 が返却される。
		 * @note	簡単にするためzig-zigステップは実装していない。
		 */
		static Node<K_, V_>*
		Find(Node<K_, V_>*& n,
			 const K_& k)
			{
				bool r(false);

				if (n) {
					Node<K_, V_>* c;
					if (k != n->k_) {
						int i = (int)(k > n->k_);
						c = Find(n->c_[i], n, k, i, r);
						if (!r) return 0;
						n->c_[1-i] = c;
					}
				}

				return n;
			}

		/**
		 * スプレー木を出力
		 * @param[in]	n	スプレー木のルート・ノード
		 * @param[in,out]	file	出力先
		 * @param[in]	c	ノードのプレフィックス
		 * @param[in]	d	ノードの深さ
		 */
		static void
		Print(const Node<K_, V_>* n,
			  FILE* file = stdout,
			  char c = 'C',
			  size_t d = 0)
			{
				if (n) {
					if (n->c_[1]) Print(n->c_[1], file, 'R', d + 1);
					for (size_t i(0); i < d; ++i) std::fprintf(file, "  ");
					std::fprintf(file, "%c:%G\n", c, (double)n->k_);
					if (n->c_[0]) Print(n->c_[0], file, 'L', d + 1);
				}
			}
	};
};

#endif	// __SPLAY_TREE_HPP__
