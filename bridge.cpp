#include <string>

// 원본의 main은 터미널 입력용이므로 웹에서는 실행하지 않습니다.
// 원본 파일 자체를 수정하는 것은 아닙니다.
#define main bst_console_main
#include "2026-FP-REPORT1.cpp"
#undef main

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#define WEB_API EMSCRIPTEN_KEEPALIVE
#else
#define WEB_API
#endif

namespace {
    // 웹에서 사용할 실제 C++ 트리
    Node<int>* root = nullptr;

    // JavaScript에 전달할 JSON 문자열
    std::string snapshot;

    void appendTreeJson(const Node<int>* node, std::string& result) {
        if (node == nullptr) {
            result += "null";
            return;
        }

        result += "{\"key\":";
        result += std::to_string(node->__key_);

        result += ",\"left\":";
        appendTreeJson(node->__left_, result);

        result += ",\"right\":";
        appendTreeJson(node->__right_, result);

        result += "}";
    }
}

// JavaScript에서 호출할 함수들
extern "C" {

    // 성공: 1, 중복: 0, 범위 오류: -1
    WEB_API int bst_insert(int key) {
        if (key < 1 || key > 1000) {
            return -1;
        }

        // 원본 삽입 알고리즘 호출
        const auto result = __insertBST(root, key);

        return result.second ? 1 : 0;
    }

    // 성공: 1, 키 없음: 0, 범위 오류: -1
    WEB_API int bst_erase(int key) {
        if (key < 1 || key > 1000) {
            return -1;
        }

        // 원본 삭제 알고리즘은 분리한 노드를 반환합니다.
        Node<int>* removed = __eraseBST(root, key);

        if (removed == nullptr) {
            return 0;
        }

        // 분리된 노드를 한 번만 해제합니다.
        delete removed;

        return 1;
    }

    WEB_API void bst_clear() {
        __clear(root);
    }

    // 현재 C++ 트리를 JSON으로 전달합니다.
    WEB_API const char* bst_snapshot() {
        snapshot.clear();
        appendTreeJson(root, snapshot);

        return snapshot.c_str();
    }
}