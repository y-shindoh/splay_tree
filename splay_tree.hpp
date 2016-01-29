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
	 * スプレー木
	 * @note	テンプレート引数 @a KTYPE にはキーの型を適用する。
	 * @note	テンプレート引数 @a VTYPE には値の型を適用する。
	 * @note	テンプレート引数 @a INVALID_K にはキーの不正値を適用する。
	 * @note	テンプレート引数 @a INVALID_V には値の不正値を適用する。
	 */
	template<typename KTYPE, typename VTYPE, KTYPE INVALID_K = ~(KTYPE)0, VTYPE INVALID_V = ~(VTYPE)0>
	class SplayTree
	{
	private:

		/**
		 * スプレー木のノード
		 */
		template<typename K_, typename V_>
		class Node
		{
		private:

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
			 * @return	探索後の処理対象ノード (探索失敗時は 0 が返る)
			 * @note	簡単にするためzig-zigステップは実装していない。
			 */
			static Node<K_, V_>*
			Find(Node<K_, V_>* n,
				 Node<K_, V_>*& p,
				 const K_& k,
				 int i)
				{
					Node<K_, V_>* c(0);

					if (n) {
						if (k != n->k_) {
							int j = (int)(k > n->k_);
							c = Find(n->c_[j], n, k, j);
							if (!c) return 0;	// not found
							n->c_[1-j] = c;
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
					if (n) {
						Node<K_, V_>* c;
						if (k != n->k_) {
							int i = (int)(k > n->k_);
							c = Find(n->c_[i], n, k, i);
							if (!c) return 0;
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
				  FILE* file,
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

		Node<KTYPE, VTYPE>* tree_;	///< 木の本体
		size_t length_;				///< 木の要素数

	public:

		/**
		 * コンストラクタ
		 */
		SplayTree()
			: tree_(0), length_(0)
			{
				;
			}

		/**
		 * デストラクタ
		 */
		virtual
		~SplayTree()
			{
				Node<KTYPE, VTYPE>::DeleteAll(tree_);
			}

		/**
		 * 要素を追加
		 * @param[in]	key	追加する要素のキー
		 * @param[in]	value	追加する要素の値
		 */
		void
		add(const KTYPE& key,
			const VTYPE& value)
			{
				tree_ = Node<KTYPE, VTYPE>::Add(tree_, key, value);
				++length_;
			}

		/**
		 * 要素を削除
		 * @param[in]	key	削除する要素のキー
		 * @return	削除した要素の値
		 * @note	削除失敗時には @a INVALID_V が返却される。
		 */
		VTYPE
		remove(const KTYPE& key)
			{
				Node<KTYPE, VTYPE>* node = Node<KTYPE, VTYPE>::Find(tree_, key);
				VTYPE r(INVALID_V);

				if (node) {
					r = node->v_;
					tree_ = Node<KTYPE, VTYPE>::DeleteRoot(tree_);
					--length_;
				}

				return r;
			}

		/**
		 * 要素を探索
		 * @param[in]	key	探索する要素のキー
		 * @return	削除した要素の値
		 * @note	削除失敗時には @a INVALID_V が返却される。
		 */
		VTYPE
		find(const KTYPE& key)
			{
				Node<KTYPE, VTYPE>* node = Node<KTYPE, VTYPE>::Find(tree_, key);

				if (node) return node->v_;
				return INVALID_V;
			}

		/**
		 * 木の根要素のキーを取得
		 * @return キー
		 * @note	取得失敗時には @a INVALID_K が返却される。
		 */
		KTYPE
		root_key() const
			{
				if (tree_) return INVALID_K;
				return tree_->k_;
			}

		/**
		 * 木の中の最小キーを取得
		 * @return 最小キー
		 * @note	取得失敗時には @a INVALID_K が返却される。
		 */
		KTYPE
		min_key() const
			{
				if (tree_) return INVALID_K;

				Node<KTYPE, VTYPE>* node(tree_);
				while (node->c_[0]) node = node->c_[0];
				return node->k_;
			}

		/**
		 * 木の中の最大キーを取得
		 * @return 最大キー
		 * @note	取得失敗時には @a INVALID_K が返却される。
		 */
		KTYPE
		max_key() const
			{
				if (tree_) return INVALID_K;

				Node<KTYPE, VTYPE>* node(tree_);
				while (node->c_[1]) node = node->c_[1];
				return node->k_;
			}

		/**
		 * 木の中の要素数を取得
		 * @return	要素数
		 */
		size_t
		size() const
			{
				return length_;
			}

		/**
		 * 木の状態を出力
		 * @param[in,out]	file	出力先
		 */
		void
		print(FILE* file = stdout) const
			{
				Node<KTYPE, VTYPE>::Print(tree_, file);
			}

		/**
		 * キーの不正値 @a INVALID_K を取得
		 * @return	キーの不正値 @a INVALID_K
		 */
		static KTYPE
		InvalidKey()
			{
				return INVALID_K;
			}

		/**
		 * 値の不正値 @a INVALID_V を取得
		 * @return	キーの不正値 @a INVALID_V
		 */
		static VTYPE
		InvalidValue()
			{
				return INVALID_V;
			}
	};
};

#endif	// __SPLAY_TREE_HPP__
