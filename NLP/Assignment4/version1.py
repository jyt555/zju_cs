import json
import pandas as pd
import re
import time
from openai import OpenAI
import httpx
from tqdm import tqdm

# 配置硅基流动API参数
SIGFLOW_API_KEY = ""
SIGFLOW_BASE_URL = "https://api.siliconflow.cn/v1"

# 数据集路径
GSM8K_PATH = "cot/gsm8k.parquet"
COMMONSENSEQA_PATH = "cot/cqa.parquet"

def load_dataset(dataset_type, max_samples=50):
    """加载数据集并预处理"""
    if dataset_type == "gsm8k":
        df = pd.read_parquet(GSM8K_PATH).head(max_samples)
        inputs = df['question'].tolist()
        labels = [answer.split("####")[-1].strip() for answer in df['answer']]
    else:  # commonsenseqa
        df = pd.read_parquet(COMMONSENSEQA_PATH).head(max_samples)
        inputs = []
        for _, row in df.iterrows():
            # 拼接问题、选项和文本
            question = f"{row['question']} 选项如下: {row['choices']}"
            inputs.append(question)
        labels = [row['answerKey'] for _, row in df.iterrows()]
    
    return inputs, labels

def cot_inference(question, dataset_type="gsm8k", max_retries=3):
    """
    执行CoT推理
    :param question: 问题文本
    :param dataset_type: 数据集类型 ('gsm8k' 或 'commonsenseqa')
    :param max_retries: 最大重试次数
    :return: 最终答案
    """
    client = OpenAI(
        api_key=SIGFLOW_API_KEY,
        base_url=SIGFLOW_BASE_URL,
        timeout=httpx.Timeout(30.0, connect=15.0)  # 增加超时时间
    )
    
    # 根据数据集类型选择提示模板
    if dataset_type == "gsm8k":
        answer_prompt = "Therefore, the answer (arabic numerals) is"
    else:  # commonsenseqa
        answer_prompt = "Therefore, among A through E, the answer is"
    
    # 第一步：生成推理步骤
    reasoning = ""
    for attempt in range(max_retries):
        try:
            step1_response = client.chat.completions.create(
                model="Qwen/QwQ-32B",
                messages=[
                    {"role": "system", "content": "You are a helpful assistant."},
                    {"role": "user", "content": f"{question} Let's think step by step."}
                ],
                temperature=0.7,
                max_tokens=20480,  # 增加token限制
                top_p=0.7,
                frequency_penalty=0.5,
                n=1
            )
            reasoning = step1_response.choices[0].message.content
            break  # 成功则跳出重试循环
        except Exception as e:
            print(f"API错误 (尝试 {attempt+1}/{max_retries}): {e}")
            if attempt == max_retries - 1:
                return f"API请求失败: {e}"
    
    # 第二步：生成最终答案
    step2_prompt = f"{question}\n\n推理步骤:\n{reasoning}\n\n{answer_prompt}"
    
    for attempt in range(max_retries):
        try:
            step2_response = client.chat.completions.create(
                model="Qwen/QwQ-32B",
                messages=[
                    {"role": "system", "content": "你是一个乐于助人的助手"},
                    {"role": "user", "content": step2_prompt}
                ],
                temperature=0.2,  # 降低随机性
                max_tokens=128,
                top_p=0.7,
                frequency_penalty=0.5,
                n=1
            )
            return step2_response.choices[0].message.content
        except Exception as e:
            print(f"API错误 (尝试 {attempt+1}/{max_retries}): {e}")
            if attempt == max_retries - 1:
                return f"API请求失败: {e}"

def extract_answer(prediction, dataset_type):
    """从模型输出中提取答案"""
    if dataset_type == "gsm8k":
        # 提取最后一个阿拉伯数字
        numbers = re.findall(r'\d+', prediction)
        return numbers[-1] if numbers else None
    else:  # commonsenseqa
        # 提取第一个大写字母
        letters = re.findall(r'[A-E]', prediction)
        return letters[0] if letters else None

def evaluate_dataset(dataset_type, max_samples=50):
    """评估数据集并计算准确率"""
    # 加载数据集
    questions, labels = load_dataset(dataset_type, max_samples)
    predictions = []
    extracted_answers = []
    
    print(f"\n开始评估 {dataset_type.upper()} 数据集 ({max_samples}个样本)...")
    
    # 对每个问题执行推理
    for i, question in enumerate(tqdm(questions, desc="处理问题")):
        prediction = cot_inference(question, dataset_type)
        predictions.append(prediction)
        
        # 从模型输出中提取答案
        extracted = extract_answer(prediction, dataset_type)
        extracted_answers.append(extracted)
        
        # 显示进度
        print(f"\n问题 {i+1}/{len(questions)}:")
        print(f"问题: {question}")
        print(f"模型输出: {prediction}")
        print(f"提取的答案: {extracted}")
        print(f"标签答案: {labels[i]}")
        
        # 添加延迟以避免API限制
        time.sleep(1)
    
    # 计算准确率
    correct = 0
    for extracted, label in zip(extracted_answers, labels):
        if extracted and extracted == label:
            correct += 1
    
    accuracy = correct / len(labels) * 100
    
    print(f"\n{dataset_type.upper()} 数据集评估结果:")
    print(f"样本数量: {len(labels)}")
    print(f"正确数量: {correct}")
    print(f"准确率: {accuracy:.2f}%")
    
    # 保存结果
    results = pd.DataFrame({
        "question": questions,
        "label": labels,
        "prediction": predictions,
        "extracted_answer": extracted_answers
    })
    results.to_csv(f"{dataset_type}_results.csv", index=False)
    
    return accuracy

def test_api_connection():
    print("测试API连接...")
    try:
        client = OpenAI(api_key=SIGFLOW_API_KEY, base_url=SIGFLOW_BASE_URL)
        response = client.chat.completions.create(
            model="Qwen/QwQ-32B",
            messages=[{"role": "user", "content": "Hello"}],
            max_tokens=1
        )
        print("API连接成功!")
        return True
    except Exception as e:
        print(f"API连接测试失败: {e}")
        print("请检查: 1. API密钥是否正确 2. 网络连接 3. 端点URL是否正确")
        return False

if __name__ == "__main__":
    if not test_api_connection():
        exit(1)
    
    # 评估GSM8K数据集
    gsm8k_accuracy = evaluate_dataset("gsm8k", max_samples=50)
    
    # 评估CommonsenseQA数据集
    commonsenseqa_accuracy = evaluate_dataset("commonsenseqa", max_samples=50)
    
    print("\n实验总结:")
    print(f"GSM8K准确率: {gsm8k_accuracy:.2f}%")
    print(f"CommonsenseQA准确率: {commonsenseqa_accuracy:.2f}%")