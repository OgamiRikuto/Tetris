# !/bin/bash

: << EOF
------------------------------------------------------------------------------------
テトリスゲームをコンパイルするためのシェルスクリプトです。
gccを用いてコンパイルします。

準備：テトリスの「ルートディレクトリ」(Makefileがあるディレクトリ)にこのファイルを置き、実行権限を与えます。
$ chmod +x compile.sh

実行：ルートディレクトリにてシェルスクリプトを実行してください。
$ compile
-----------------------------------------------------------------------------------
EOF

#!/bin/bash

# --- OS 判定して実行ファイル名切り替え ---
if [[ "$OSTYPE" == "msys" || "$OSTYPE" == "cygwin" || "$OSTYPE" == "win32" ]]; then
    TARGET="tetris.exe"
    LINK=""
else
    TARGET="tetris"
    LINK="-lcurses"
fi

CC="gcc"
CCFLAGS="-Wall -Iincludes -g"

SRCDIR="src"

# src 以下の全 .c を取得
SRC=$(find "$SRCDIR" -name '*.c')

# .o のリスト生成
OBJ=""
for f in $SRC; do
    OBJ="$OBJ ${f%.c}.o"
done

# --- build 関数 ---
build() {
    echo "[Compile]"

    for c in $SRC; do
        o="${c%.c}.o"
        echo "  $c -> $o"
        $CC $CCFLAGS -c "$c" -o "$o" || exit 1
    done

    echo "[Link]"
    $CC $OBJ -o "$TARGET" $LINK || exit 1

    echo "[SUCCESS!]"
}

# --- clean 関数 ---
clean() {
    echo "[Clean]"
    rm -f $OBJ $TARGET
    echo "Cleaned."
}

# --- run 関数（make test 相当）---
run() {
    build
    echo "[Run] ./$TARGET"
    ./$TARGET
}

# --- メイン処理 ---
case "$1" in
    test)
        run
        ;;
    clean)
        clean
        ;;
    *)
        build
        ;;
esac
