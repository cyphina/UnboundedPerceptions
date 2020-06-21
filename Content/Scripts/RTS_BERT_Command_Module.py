
import os 
from tokenization import FullTokenizer
import tensorflow as tf

import unreal_engine as ue 
from TFPluginAPI import TFPluginAPI

def bertPreprocess(inputString, tokenizer):
    tokens = []
    tokens.append('[CLS]')
    tokens += tokenizer.tokenize(Sample_Str)
    tokens.append('[SEP]')

    ids = tokenizer.convert_tokens_to_ids(tokens)
    input_mask = [1] * len(ids)
    segment_ids = [0] * MAX_SEQ_LEN

    MAX_SEQ_LEN = 128
    while len(ids) < MAX_SEQ_LEN:
        ids.append(0)
        input_mask.append(0)

    label_ids = ids
    t_inputs = tf.constant([ids])
    t_mask = tf.constant([input_mask])
    t_segment = tf.constant([segment_ids])
    t_labels = tf.constant([label_ids])

    return t_labels, t_segment, t_inputs, t_mask

class BERTModule(TFPluginAPI):
    
    def onSetup(self):
        BERT_DIR = os.path.join(os.path.curdir, "BertModel")
        self.imported = tf.saved_model.load(BERT_DIR)
        self.f = self.imported.signatures["serving_default"]
        VOCAB_PATH = os.path.join(BERT_DIR, "assets", "vocab.txt")
        self.tokenizer = FullTokenizer(VOCAB_PATH)

    def onJsonInput(self, jsonInput):
        print(jsonInput)
        input_string = jsonInput['text']
        label_ids, segment_ids, input_ids, input_mask = bertPreprocess(input_string, self.tokenizer)
        res = f(label_ids=label_ids, segment_ids=segment_ids, input_ids=input_ids, input_mask=input_mask)
        return {'prediction':res['labels'].numpy()}

def getApi():
    return BERTModule.getInstance()

# #%%
# #Load BERT model fine-tuned to group/split commands
# BERT_DIR = os.path.join(os.path.curdir, "BertModel")
# imported = tf.saved_model.load(BERT_DIR)
# f = imported.signatures["serving_default"]

# #%%
# #Load tokenizer with wordpiece vocabulary generated from fine-tuned model
# VOCAB_PATH = os.path.join(BERT_DIR, "assets", "vocab.txt")
# tokenizer = FullTokenizer(VOCAB_PATH)

# #%%
# #Test out or estimator 
# Sample_Str = "Come closer!"
# tokens = []
# tokens.append('[CLS]')
# tokens += tokenizer.tokenize(Sample_Str)
# tokens.append('[SEP]')

# ids = tokenizer.convert_tokens_to_ids(tokens)
# input_mask = [1] * len(ids)
# segment_ids = [0] * MAX_SEQ_LEN

# MAX_SEQ_LEN = 128
# while len(ids) < MAX_SEQ_LEN:
#     ids.append(0)
#     input_mask.append(0)

# label_ids = ids
# t_ids = tf.constant([ids])
# t_mask = tf.constant([input_mask])
# t_segment = tf.constant([segment_ids])
# t_labels = tf.constant([label_ids])

# res = f(label_ids=t_labels, segment_ids=t_segment, input_ids=t_ids, input_mask=t_mask)

# print(Sample_Str + " is associated with - ")
# if not res['labels'].numpy():
#     print("Grouping")
# else:
#     print("Splitting")
