import json
import re
import random
from openai import OpenAI
import time  # 添加时间模块用于监控执行时间

# 配置阿里云百炼API
client = OpenAI(api_key="",
                base_url="https://dashscope.aliyuncs.com/compatible-mode/v1")

# 调用API得到回答
def generate_response(messages, temperature=0.7, max_tokens=1024):
    """通过API调用生成回复"""
    try:
        start_time = time.time()  # 记录开始时间
        completion = client.chat.completions.create(
            model="qwen3-30b-a3b", 
            messages=messages,
            temperature=temperature,
            max_tokens=max_tokens,
            extra_body={"enable_thinking": False}
        )
        elapsed = time.time() - start_time
        # print(f"✓ API调用成功 | 耗时: {elapsed:.2f}秒 | Token: {max_tokens} | 温度: {temperature}")
        return completion.choices[0].message.content
    except Exception as e:
        print(f"✗ API调用出错: {e}")
        return "API调用失败，请稍后再试。"

# ======================== Step-Aware Verifier 模块 ========================
def split_reasoning_steps(reasoning):
    """将推理文本分割为独立步骤"""
    delimiters = r"\n\d+[.\)] |\n- |\n• |\n\d+\) |; |\n"
    steps = re.split(delimiters, reasoning)
    return [s.strip() for s in steps if s.strip()]

def verify_step(question, step_content):
    """验证单个推理步骤的正确性（英文提示）"""
    # print(f"🔍 验证推理步骤 | 步骤内容: {step_content[:50]}...")
    prompt = [
        {
            "role": "system", 
            "content": "You are a strict math teacher evaluating a student's solution step. Please rigorously check the correctness of the following reasoning step:"
        },
        {
            "role": "user",
            "content": f"Question: {question}\nReasoning step: {step_content}\n\n"
                      "Please respond in the following format:\n"
                      "1. Correctness score (0.0-1.0):\n"
                      "2. Error analysis (if applicable):\n"
                      "3. Improvement suggestion (if applicable):"
        }
    ]
    return generate_response(prompt, temperature=0.3, max_tokens=256)

def parse_verification_response(response):
    """解析验证器返回的响应（英文关键词）"""
    score_match = re.search(r"Correctness score[:\s]*([0-9.]+)", response)
    analysis_match = re.search(r"Error analysis[:\s]*(.+)", response, re.DOTALL)
    suggestion_match = re.search(r"Improvement suggestion[:\s]*(.+)", response, re.DOTALL)
    
    score = float(score_match.group(1)) if score_match else 0.0
    analysis = analysis_match.group(1).strip() if analysis_match else ""
    suggestion = suggestion_match.group(1).strip() if suggestion_match else ""
    
    print(f"  验证结果 | 评分: {score:.2f} | 分析: {analysis[:50]}{'...' if len(analysis) > 50 else ''}")
    return {
        "score": score,
        "analysis": analysis,
        "suggestion": suggestion
    }

def verify_full_path(question, reasoning_path):
    """验证完整推理路径"""
    print(f"🔍 开始验证完整推理路径 | 路径长度: {len(reasoning_path)}字符")
    total_score = 0
    valid_steps = 0
    verification_report = []
    
    steps = split_reasoning_steps(reasoning_path)
    print(f"  分割为 {len(steps)} 个独立步骤")
    
    for i, step in enumerate(steps):
        # print(f"  步骤 {i+1}/{len(steps)} | 内容: {step[:30]}{'...' if len(step) > 30 else ''}")
        verification = verify_step(question, step)
        parsed = parse_verification_response(verification)
        total_score += parsed["score"]
        valid_steps += 1 if parsed["score"] > 0.7 else 0
        
        verification_report.append({
            "step": i+1,
            "content": step,
            "verification": parsed
        })
    
    path_confidence = total_score / len(steps) if steps else 0
    step_accuracy = valid_steps / len(steps) if steps else 0
    
    print(f"✅ 路径验证完成 | 置信度: {path_confidence:.2f} | 步骤准确率: {step_accuracy:.2f}")
    return {
        "confidence": path_confidence,
        "step_accuracy": step_accuracy,
        "report": verification_report
    }

def correct_reasoning(question, original_reasoning, verification):
    """修正推理路径中的错误步骤"""
    error_steps = [s for s in verification["report"] if s["verification"]["score"] < 0.7]
    
    if not error_steps:
        print("🟢 无需修正 - 所有步骤评分均高于0.7")
        return original_reasoning
    
    print(f"⚠️ 需要修正 - {len(error_steps)}个低分步骤")
    error_info = [{
        "step": step["content"],
        "analysis": step["verification"]["analysis"],
        "suggestion": step["verification"]["suggestion"]
    } for step in error_steps]
    
    prompt = [
        {
            "role": "system",
            "content": "You are correcting errors in mathematical reasoning. Below is the original question, reasoning process, and verification report:"
        },
        {
            "role": "user",
            "content": f"Question: {question}\n"
                      f"Original reasoning:\n{original_reasoning}\n\n"
                      f"Verification report (error steps):\n{json.dumps(error_info, ensure_ascii=False)}\n\n"
                      "Please generate a corrected reasoning process, preserving correct steps and correcting those marked as errors."
        }
    ]
    
    return generate_response(prompt, temperature=0.5, max_tokens=1024)
# ======================== Step-Aware Verifier 模块结束 ========================

# 添加了多路径推理、批处理和Step-Aware Verifier的CoT推理
def cot_Qwen(questions, trigger_phrase="Let's think step by step.", path_num=3, batch_size=16, enable_verifier=True):
    """CoT推理函数，包含Step-Aware Verifier"""
    print("\n" + "="*60)
    print(f"🚀 开始CoT推理 | 问题数: {len(questions)} | 路径数: {path_num} | Verifier: {'启用' if enable_verifier else '禁用'}")
    print("="*60)
    
    correct = 0
    total = 0
    step1_replies = []
    gold_answers = []
    full_questions = []
    
    # 第一步：生成推理步骤
    for i, q in enumerate(questions): 
        data = json.loads(q.strip())
        question = data['question']
        options = "\n".join(data['options'])
        full_question = f"{question}\nOptions:\n{options}"
        full_questions.append(full_question)
        gold_answers.append(data['correct'])

        step1_prompt = [
            {"role": "system", "content": "You are a helpful assistant."},
            {"role": "user", "content": f"{full_question} {trigger_phrase}"}
        ]

        # print(f"\n🔄 生成推理路径 | 问题 {i+1}/{len(questions)}: {question[:30]}{'...' if len(question) > 30 else ''}")
        for path_i in range(path_num):
            temperature = 0.7 + 0.3 * path_i
            # print(f"  路径 {path_i+1}/{path_num} | 温度: {temperature:.1f}")
            reply = generate_response(step1_prompt, temperature, max_tokens=1024)
            step1_replies.append(reply)
    
    answers = []
    candidate_lists = []
    
    # 第二步：生成最终答案
    for idx in range(len(questions)): 
        q = full_questions[idx]
        print(f"\n🔍 处理问题 {idx+1}/{len(questions)}: {q.splitlines()[0][:30]}{'...' if len(q) > 30 else ''}")
        
        candidates_reply = step1_replies[idx * path_num: (idx + 1) * path_num]
        verified_replies = []
        
        # 应用Step-Aware Verifier
        for j, reply in enumerate(candidates_reply):
            print(f"  🔄 处理路径 {j+1}/{len(candidates_reply)} | 长度: {len(reply)}字符")
            
            if enable_verifier:
                verification = verify_full_path(q, reply)
                
                if verification["step_accuracy"] < 0.8:
                    print(f"  ⚠️ 路径 {j+1}准确率低 ({verification['step_accuracy']:.2f}) - 尝试修正")
                    corrected_reply = correct_reasoning(q, reply, verification)
                    verified_replies.append({
                        "original": reply,
                        "corrected": corrected_reply,
                        "verification": verification,
                        "is_corrected": True
                    })
                else:
                    print(f"  ✅ 路径 {j+1}准确率良好 ({verification['step_accuracy']:.2f}) - 直接使用")
                    verified_replies.append({
                        "original": reply,
                        "corrected": reply,
                        "verification": verification,
                        "is_corrected": False
                    })
            else:
                verified_replies.append({
                    "original": reply,
                    "corrected": reply,
                    "is_corrected": False
                })
        
        # 生成最终答案
        candidates_res = []
        for k, v_reply in enumerate(verified_replies):
            print(f"  💡 生成最终答案 | 路径 {k+1}/{len(verified_replies)}")
            step2_prompt = [
                {"role": "system", "content": "You are a helpful assistant."},
                {"role": "user", "content": f"{q} {trigger_phrase} {v_reply['corrected']} Therefore, among A through E, the answer is"}
            ]
            answer = generate_response(step2_prompt, 0.7, max_tokens=1024)
            candidates_res.append(answer)
        
        final, candidates = get_final_res(candidates_res)
        candidate_lists.append(candidates)
        answers.append(final)
    
    # 输出结果
    for i, (question, candidate_answers, gold_answer, pred_answer) in enumerate(zip(full_questions, candidate_lists, gold_answers, answers)):
        print(f"\n📊 结果 {i+1}/{len(questions)}")
        print(f"  问题: {question.splitlines()[0][:30]}{'...' if len(question) > 30 else ''}")
        print(f"  候选答案: {candidate_answers}")
        print(f"  正确答案: {gold_answer}, 预测答案: {pred_answer}")
        
        if pred_answer and pred_answer.upper() == gold_answer.upper():
            print(f"  ✅ 正确!")
            correct += 1
        else:
            print(f"  ❌ 错误!")
        total += 1
        
    print(f"\n🎯 本批次完成 | 正确: {correct}/{total} | 准确率: {correct/total:.2f}" if total > 0 else "本批次无有效问题")
    return total, correct

# 从候选的多路径推理结果中匹配出候选结果并选出唯一最终结果
def get_final_res(candidates_res):
    """从多个答案中确定最终答案"""
    pred_answers = []
    for candidate in candidates_res:
        pred_answer = None
        if candidate:
            bold_match = re.search(r'\*\*(?:.*?)([A-E])\)[^\*]*\*\*', candidate)
            if bold_match:
                pred_answer = re.search(r'([A-E])\)', bold_match.group()).group(1)
            else:
                simple_match = re.search(r'\b([A-E])\b', candidate)
                if simple_match:
                    pred_answer = simple_match.group(1)
        pred_answers.append(pred_answer)
    
    valid_answers = [ans for ans in pred_answers if ans is not None]
    
    if valid_answers:
        final = max(set(valid_answers), key=valid_answers.count)
        print(f"  最终答案确定: {final} (来自 {valid_answers.count(final)}/{len(valid_answers)} 路径)")
        return final, pred_answers
    else:
        print("⚠️ 无法确定最终答案 - 所有路径均无有效答案")
        return None, pred_answers

def evaluate_json_questions_with_api(json_path='AQuA/test.json', max_samples=None, path_num=3, batch_size=16, enable_verifier=True):
    """
    评估JSON格式的题目数据，包含Step-Aware Verifier选项
    """
    print(f"\n{'='*60}\n🔍 开始评估数据集 | 文件: {json_path}\n{'='*60}")
    correct = 0
    total = 0

    with open(json_path, 'r', encoding='utf-8') as f:
        all_data = f.readlines()
    
    random.seed(42)
    if max_samples:
        selected_questions = random.sample(all_data, min(max_samples, len(all_data)))
    else:
        selected_questions = all_data
    
    num_questions = len(selected_questions)
    print(f"📂 数据集加载完成 | 总问题数: {len(all_data)} | 采样数: {num_questions}")
    print(f"⚙️ 配置 | 路径数: {path_num} | 批次大小: {batch_size} | Verifier: {'启用' if enable_verifier else '禁用'}")

    for batch_idx, start_idx in enumerate(range(0, num_questions, batch_size)):
        batch_q = selected_questions[start_idx: start_idx + batch_size]
        print(f"\n{'='*50}")
        print(f"🔧 处理批次 {batch_idx+1}/{(num_questions-1)//batch_size+1} | 问题: {start_idx+1}-{min(start_idx+batch_size, num_questions)}")
        print(f"{'='*50}")

        try:            
            total_batch, correct_batch = cot_Qwen(
                batch_q,
                "Let's Think Step By Step", 
                path_num, 
                batch_size,
                enable_verifier
            )

            total += total_batch
            correct += correct_batch
            print(f"✅ 批次完成 | 本批正确: {correct_batch}/{total_batch} | 累计正确: {correct}/{total}")
        except KeyError as e:
            print(f"❌ 数据异常: 缺少关键字段 {e}")
        except Exception as e:
            print(f"❌ 处理异常: {e}")
    
    if total > 0:
        accuracy = correct / total
        print(f"\n{'='*60}\n🏁 评估完成 | 总正确: {correct}/{total} | 准确率: {accuracy:.4f}\n{'='*60}")
        return accuracy
    else:
        print("⚠️ 评估失败 - 无有效问题处理")
        return 0

# 示例使用方式
if __name__ == "__main__":
    # 运行评估（启用Verifier）
    evaluate_json_questions_with_api(
        json_path='AQuA/test.json',
        max_samples=10,
        batch_size=3,
        path_num=3,
        enable_verifier=True
    )
    
    # # 运行评估（禁用Verifier）
    # evaluate_json_questions_with_api(
    #     json_path='AQuA/test.json',
    #     max_samples=10,
    #     batch_size=3,
    #     path_num=3,
    #     enable_verifier=False
    # )