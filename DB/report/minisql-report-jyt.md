











<center><span style="font-size:2rem">MiniSQL个人报告</span></center>





<center><span style="font-size:1.5rem">3220103450 姜雨童</span></center>

<center><span style="font-size:1rem">Date：2024.06.24</span></center>

<div style="page-break-after: always"></div>

<!-- 注释语句：导出PDF时会在这里分页 -->

[TOC]



## 完成模块说明

1. 参与小组讨论，对MiniSQL的实现进行分析
2. 负责模块四（Catalog Manager）和模块五（Executor）的实现

## Catalog Manager模块

### Catalog Manager概述

Catalog Manager 负责管理和维护数据库的所有模式信息，包括：

- 数据库中所有表的定义信息，包括表的名称、表中字段（列）数、主键、定义在该表上的索引。
- 表中每个字段的定义信息，包括字段类型、是否唯一等。
- 数据库中所有索引的定义，包括所属表、索引建立在那个字段上等。

这些模式信息在被创建、修改和删除后还应被持久化到数据库文件中。此外，Catalog Manager还需要为上层的执行器Executor提供公共接口以供执行器获取目录信息并生成执行计划。

### 模块调用

- 调用record manager中column，row和schema的序列化和反序列化函数
- 调用disk manager中的bufferpoolmanager，为表和索引分配单独的数据页
- 调用index manager中的BplusTreeIndex，为表中的列创建索引
- （本模块的CatalogManager类为Execute Engine提供函数接口，以实现minisql各模块功能）

### 实现的接口

#### catalog.cpp

- `uint32_t CatalogMeta::GetSerializedSize() const`
- `CatalogManager::CatalogManager(BufferPoolManager *buffer_pool_manager, LockManager *lock_manager, LogManager *log_manager, bool init)`

- `dberr_t CatalogManager::CreateTable(const string &table_name, TableSchema *schema, Txn *txn, TableInfo *&table_info)`

- `dberr_t CatalogManager::GetTable(const string &table_name, TableInfo *&table_info)`
- `dberr_t CatalogManager::GetTables(vector<TableInfo *> &tables) const`
- `dberr_t CatalogManager::CreateIndex(const std::string &table_name, const string &index_name, const std::vector<std::string> &index_keys, Txn *txn, IndexInfo *&index_info, const string &index_type)`
- `dberr_t CatalogManager::GetIndex(const std::string &table_name, const std::string &index_name, IndexInfo *&index_info) const`
- `dberr_t CatalogManager::GetTableIndexes(const std::string &table_name, std::vector<IndexInfo *> &indexes) const`
- `dberr_t CatalogManager::DropTable(const string &table_name)`

- `dberr_t CatalogManager::DropIndex(const string &table_name, const string &index_name)`

- `dberr_t CatalogManager::FlushCatalogMetaPage() const`

- `dberr_t CatalogManager::LoadTable(const table_id_t table_id, const page_id_t page_id)`

- `dberr_t CatalogManager::LoadIndex(const index_id_t index_id, const page_id_t page_id)`

- `dberr_t CatalogManager::GetTable(const table_id_t table_id, TableInfo *&table_info)`

#### index.cpp

- `uint32_t IndexMetadata::GetSerializedSize() const`（模块内调用）

#### index.h

- `void Init(IndexMetadata *meta_data, TableInfo *table_info, BufferPoolManager *buffer_pool_manager)`（模块内调用）

### 具体实现

可以把上述函数接口大致分为以下几类：

- 序列化和反序列化（包括初始化）
- Catalog Manager
- Create Table/Index
- Get Table(s)/Index/TableIndexes
- Load Table/Index
- Drop Table/Index
- Flush CatalogMetaPage

#### 序列化和反序列化

> 数据库中定义的表和索引在内存中以`TableInfo`和`IndexInfo`的形式表现。以`IndexInfo`为例，它包含了这个索引定义时的元信息`meta_data_`，该索引对应的表信息`table_info_`，该索引的模式信息`key_schema_`和索引操作对象`index_`。除元信息`meta_data_`外，其它的信息都是通过反序列化后的元信息生成的。
>
> 因此，为了能够将所有表和索引的定义信息持久化到数据库文件并在重启时从数据库文件中恢复，我们需要为表和索引的元信息`TableMetadata`和`IndexMetadata`实现序列化和反序列化操作。（为了简便处理，在序列化时我们为每一个表和索引都分配一个单独的数据页用于存储序列化数据。）`CatalogMeta`的信息将会被序列化到数据库文件的第`CATALOG_META_PAGE_ID`号数据页中（逻辑意义上），`CATALOG_META_PAGE_ID`默认值为0。

就具体实现而言，CatalogMeta类的序列化和反序列化函数都已给出，只需要实现几个获取序列化长度的函数以及Index的初始化（Init）。代码实现将统一附在报告最后。

#### Catalog Manager

> `CatalogManager`能够在数据库实例（`DBStorageEngine`）初次创建时（`init = true`）初始化元数据；并在后续重新打开数据库实例时，从数据库文件中加载所有的表和索引信息，构建`TableInfo`和`IndexInfo`信息置于内存中。

根据数据库的调用情况，可以分为首次创建数据库和重新加载数据库两种情况。

首次调用时，只需要catalogmanager进行初始化。而重新加载时，则要通过反序列化得到数据库的元信息。接着通过对catalogmeta中信息的遍历，反序列化得到tablemetadata和indexmetadata，构建对应的tableinfo和indexinfo并把信息加入catalogmanager中，从而加载出完整的catalogmanager。

#### Create Table/Index

这两个构建函数的实现逻辑类似，这里以create table为例。首先需要利用bufferpoolmanager获取一个空闲页作为新建表的数据页（对应page_id），而调用catalogmeta中GetNextTableId得到的是新建表的id。然后调用初始化函数生成tableinfo，并把新建的表存到table列表中。最后更新catalogmetapage的信息，保证下一次调用时信息是正确的。（注意在实现create table的时候要进行深度拷贝，否则会报错。）

#### Get Table(s)/Index/TableIndexes

同样以get table为例，根据给出的tablename（或是tableid，这里实现了通过name和id两个不同标识查找表的函数）在table列表table_中找到对应tableinfo即可。而对于获得多个信息的get tables/tableindexes，只需创建向量表存入信息。

#### Load Table/Index

以load table为例。根据表id得到数据页id，然后通过反序列化得到tablemeta，后续把tablemeta中的数据，如tablename，tableinfo载入到数据库中。

#### Drop Table/Index

以drop table为例，根据表名，在catalogmanager类中table名称向量表和table向量表中删除该表的各类信息。同时要在catalogmeta中删除该表，并调用bufferpoolmanager类的deletepage函数对该表所在的页面进行回收，避免空间上的浪费。

#### Flush CatalogMetaPage

把目前catalogmetadata写入数据页，并即时将数据序列化到磁盘中。

## Planner and executor模块

### Planner and executor概述

本实验主要包括Planner和Executor两部分。

Planner的主要功能是将解释器（Parser）生成的语法树，改写成数据库可以理解的数据结构。在这个过程中，我们会将所有sql语句中的标识符（Identifier）解析成没有歧义的实体，即各种C++的类，并通过Catalog Manager 提供的信息生成执行计划。（这部分没有代码需要实现，只需要了解parser的实现方式，便于后续编写代码。）

Executor遍历查询计划树，将树上的 PlanNode 替换成对应的 Executor，随后调用 Record Manager、Index Manager 和 Catalog Manager 提供的相应接口进行执行，并将执行结果返回给上层模块。

### 模块调用

- 调用catalog.cpp中CatalogManager类的各函数，以实现minisql各模块的功能

### 实现的接口

#### execute_engine.cpp

这些函数均由类内函数`ExecuteEngine::Execute`（已给出实现）统一调用

- `dberr_t ExecuteEngine::ExecuteCreateTable(pSyntaxNode ast, ExecuteContext *context)`
- `dberr_t ExecuteEngine::ExecuteDropTable(pSyntaxNode ast, ExecuteContext *context)`
- `dberr_t ExecuteEngine::ExecuteShowIndexes(pSyntaxNode ast, ExecuteContext *context)`
- `dberr_t ExecuteEngine::ExecuteCreateIndex(pSyntaxNode ast, ExecuteContext *context)`
- `dberr_t ExecuteEngine::ExecuteDropIndex(pSyntaxNode ast, ExecuteContext *context)`
- `dberr_t ExecuteEngine::ExecuteExecfile(pSyntaxNode ast, ExecuteContext *context)`
- `dberr_t ExecuteEngine::ExecuteQuit(pSyntaxNode ast, ExecuteContext *context)`

### 具体实现

#### Parser语法分析

上述功能函数传入的参数均含语法树的结点`pSyntaxNode ast`，因此，在正式实现函数代码前，要先分析parser语法树的实现。

对于简单的sql语句，如drop table，并不怎么需要分析语法树，因此这里以create index为例。在`src/include/parser/minisql.y`下找到对应语法，如下：

```yacas
sql_create_index:
  CREATE INDEX IDENTIFIER ON IDENTIFIER '(' column_list ')' {
    $$ = CreateSyntaxNode(kNodeCreateIndex, NULL);
    SyntaxNodeAddChildren($$, $3);
    SyntaxNodeAddChildren($$, $5);
    pSyntaxNode index_keys_node = CreateSyntaxNode(kNodeColumnList, "index keys");
    SyntaxNodeAddChildren(index_keys_node, $7);
    SyntaxNodeAddChildren($$, index_keys_node);
  }
  | CREATE INDEX IDENTIFIER ON IDENTIFIER '(' column_list ')' USING IDENTIFIER {
      $$ = CreateSyntaxNode(kNodeCreateIndex, NULL);
      SyntaxNodeAddChildren($$, $3);
      SyntaxNodeAddChildren($$, $5);
      pSyntaxNode index_keys_node = CreateSyntaxNode(kNodeColumnList, "index keys");
      SyntaxNodeAddChildren(index_keys_node, $7);
      SyntaxNodeAddChildren($$, index_keys_node);
      pSyntaxNode index_type_node = CreateSyntaxNode(kNodeIndexType, "index type");
      SyntaxNodeAddChildren(index_type_node, $10);
      SyntaxNodeAddChildren($$, index_type_node);
  }
  ;
```

不难分析出对应语法树1如下图：

![image-20240624181544467](C:/Users/Lenovo/AppData/Roaming/Typora/typora-user-images/image-20240624181544467.png)

据此可以通过函数传入的语法树结点获取sql语句蕴含的对应信息：

```c++
  // analyze syntaxNode
  auto index_name = ast->child_->val_; // see parser/minisql.y
  auto table_name = ast->child_->next_->val_;
  auto list = ast->child_->next_->next_;
```

#### ExecuteCreateTable

CreateTable, DropTable等函数实现较为类似，这里仅以CreateTabel函数为例。首先对语法树进行如上分析，获取sql指令中对应的信息。然后根据每一列给出的信息创建表内的属性。具体通过结点状态的不同（kNodeColumnList和kNodeColumnDefinition），先将primary key放入向量表中，再记录其他的key（如若遇到unique key也放入特定向量表中记录）。最后创建表和索引，并将相关信息记入。

#### Executefile

该函数需要读取文件内的sql语句，因此不仅用到了c++的文件流读取函数，还用到了上述提到的语法分析，需要在Parser模块调用`yyparse()`（一个示例在`src/main.cpp`中）完成SQL语句解析，得到语法树的根结点`pSyntaxNode`。

这个函数首先将文件内字符读到缓冲区中，以`;`为 分隔符形成一条或多条sql语句。然后利用parser进行语法解析，执行对应的操作（可以调用给出的Execute函数来进行），最后返回相关信息并关闭文件。

## 模块功能验证

### Catalog Manager

![image-20240624173440805](C:/Users/Lenovo/AppData/Roaming/Typora/typora-user-images/image-20240624173440805.png)

### Execute Engine

![image-20240624173630583](C:/Users/Lenovo/AppData/Roaming/Typora/typora-user-images/image-20240624173630583.png)

（说明：在设计实现minisql的过程中，电脑的环境配置出了一点问题，导致原来能构建的minisql现在跑不起来了。询问助教后还是没有找到解决办法，截至提交报告时，电脑环境还是跑不了。因此execute engine的测试是在室友电脑上验证的，显示路径和上面有所不同。可以通过对代码/注释的比对来验证跑的确实是我的execute engine模块。）

然而，在后续进行基本操作时，发现模块五的文件读入部分有问题，会出现“Segmentation fault”报错。截至提交报告时，仍没有发现是哪个地方出了bug。（截图如下，同样是借用了室友的电脑。）

![fca6f217482ae4caee202d951224bfc](C:/Users/Lenovo/Documents/WeChat Files/wxid_t26hqc2iurse22/FileStorage/Temp/fca6f217482ae4caee202d951224bfc.png)

## 代码实现

### Catalog Manager

#### index.h

```c++
void Init(IndexMetadata *meta_data, TableInfo *table_info, BufferPoolManager *buffer_pool_manager) {
    // Step1: init index metadata and table info
    // Step2: mapping index key to key schema
    // Step3: call CreateIndex to create the index
    meta_data_ = meta_data;
    const Schema *table_schema = table_info->GetSchema();
    key_schema_ = Schema::ShallowCopySchema(table_schema, meta_data->GetKeyMapping());
    index_ = CreateIndex(buffer_pool_manager, "bptree");
  }
```

#### index.cpp

```c++
uint32_t IndexMetadata::GetSerializedSize() const {
  return  sizeof(uint32_t)*5 + index_name_.length() + key_map_.size()*sizeof(uint32_t);
}
```

#### catalog.cpp

```c++
/**
 * TODO: Student Implement FIN OK
 */
uint32_t CatalogMeta::GetSerializedSize() const {
  return sizeof(uint32_t) * (3 + table_meta_pages_.size() * 2 + index_meta_pages_.size() * 2);
}

/**
 * TODO: Student Implement FIN OK
 */
CatalogManager::CatalogManager(BufferPoolManager *buffer_pool_manager, LockManager *lock_manager,
                               LogManager *log_manager, bool init)
    : buffer_pool_manager_(buffer_pool_manager), lock_manager_(lock_manager), log_manager_(log_manager) {
  if (init) {
    catalog_meta_ = CatalogMeta::NewInstance();
    FlushCatalogMetaPage();
  } else {
    auto meta_page = buffer_pool_manager_->FetchPage(CATALOG_META_PAGE_ID);
    catalog_meta_ = CatalogMeta::DeserializeFrom(meta_page->GetData());

    // get info of tables
    for (auto &p : catalog_meta_->table_meta_pages_) { // p.first:table_id; p.second:page_id
      TableMetadata *table_meta;
      TableMetadata::DeserializeFrom(buffer_pool_manager_->FetchPage(p.second)->GetData(), table_meta);
      auto table_heap = TableHeap::Create(buffer_pool_manager_, table_meta->GetFirstPageId(),
                                          table_meta->GetSchema(), log_manager_, lock_manager_);
      auto table_info = TableInfo::Create();
      table_info->Init(table_meta, table_heap);
      tables_[p.first] = table_info; // store table info
      table_names_[table_meta->GetTableName()] = p.first;
    }

    // get info of indexes
    for (auto &p : catalog_meta_->index_meta_pages_) { // p.first:index_id; p.second:page_id
      IndexMetadata *index_meta;
      IndexMetadata::DeserializeFrom(buffer_pool_manager_->FetchPage(p.second)->GetData(), index_meta);
      auto table_id = index_meta->GetTableId();
      auto table_info = tables_[table_id];
      auto table_name = table_info->GetTableName();

      auto index_info = IndexInfo::Create();
      index_info->Init(index_meta, table_info, buffer_pool_manager_);

      indexes_[p.first] = index_info; // store index info
      if (index_names_.find(table_name) == index_names_.end())
        index_names_.emplace(table_name, unordered_map<string, index_id_t>());
      index_names_[table_name][index_meta->GetIndexName()] = p.first; //*
    }
  }
}

/**
 * TODO: Student Implement FIN OK
 */
dberr_t CatalogManager::CreateTable(const string &table_name, TableSchema *schema, Txn *txn, TableInfo *&table_info) {
  if (table_names_.find(table_name) != table_names_.end())
    return DB_TABLE_ALREADY_EXIST;

  IndexSchema *schema_copy = Schema::DeepCopySchema(schema); //* debug

  // new id of table and page
  auto table_id = catalog_meta_->GetNextTableId(); // get new id of table
  table_names_[table_name] = table_id;
  page_id_t page_id;
  auto new_table_page = buffer_pool_manager_->NewPage(page_id); // get new id of page
  catalog_meta_->table_meta_pages_[table_id] = page_id; // set new id of table and page

  // new table info
  auto table_heap = TableHeap::Create(buffer_pool_manager_, schema_copy, txn, log_manager_, lock_manager_);
  auto table_meta = TableMetadata::Create(table_id, table_name, page_id, schema_copy);
  table_meta->SerializeTo(new_table_page->GetData());
  table_info = TableInfo::Create();
  table_info->Init(table_meta, table_heap);
  tables_[table_id] = table_info; // set new info of table
  buffer_pool_manager_->UnpinPage(page_id, true); // unpin page in buffer

  // update catalog_meta_page
  auto meta_page = buffer_pool_manager_->FetchPage(CATALOG_META_PAGE_ID);
  catalog_meta_->SerializeTo(meta_page->GetData());
  buffer_pool_manager_->UnpinPage(CATALOG_META_PAGE_ID, true);

  return DB_SUCCESS;
}

/**
 * TODO: Student Implement FIN OK
 */
dberr_t CatalogManager::GetTable(const string &table_name, TableInfo *&table_info) {
  if (table_names_.find(table_name) == table_names_.end())
    return DB_TABLE_NOT_EXIST;
  auto table_id = table_names_[table_name];
  table_info = tables_[table_id];
  return  DB_SUCCESS;
}

/**
 * TODO: Student Implement FIN OK
 */
dberr_t CatalogManager::GetTables(vector<TableInfo *> &tables) const {
  if (tables_.empty())
    return DB_TABLE_NOT_EXIST;
  for(auto &p : tables_)
    tables.emplace_back(p.second);
  return  DB_SUCCESS;
}

/**
 * TODO: Student Implement FIN OK
 */
dberr_t CatalogManager::CreateIndex(const std::string &table_name, const string &index_name,
                                    const std::vector<std::string> &index_keys, Txn *txn, IndexInfo *&index_info,
                                    const string &index_type) {
  if (table_names_.find(table_name) == table_names_.end())
    return DB_TABLE_NOT_EXIST;
  if (index_names_[table_name].find(index_name) != index_names_[table_name].end())
    return DB_INDEX_ALREADY_EXIST;

  // check whether keys are in the column
  auto table_id = table_names_[table_name];
  auto table_info = tables_.at(table_id);
  auto schema = table_info->GetSchema();
  std::vector<uint32_t> key_map; // record index_id
  for (auto &p : index_keys) {
    index_id_t index_id;
    if (schema->GetColumnIndex(p, index_id) == DB_COLUMN_NAME_NOT_EXIST)
      return DB_COLUMN_NAME_NOT_EXIST;
    key_map.emplace_back(index_id);
  }

  // new id of index and page
  auto index_id = catalog_meta_->GetNextIndexId();
  index_names_[table_name][index_name] = index_id; //* debug .at() is wrong
  page_id_t page_id;
  auto new_index_page = buffer_pool_manager_->NewPage(page_id); // get new id of page
  catalog_meta_->index_meta_pages_[index_id] = page_id; // set new id of index and page

  // new index info
  auto index_meta = IndexMetadata::Create(index_id, index_name, table_id, key_map);
  index_meta->SerializeTo(new_index_page->GetData());
  index_info = IndexInfo::Create();
  index_info->Init(index_meta, table_info, buffer_pool_manager_);
  indexes_[index_id] = index_info; // set new info of index
  buffer_pool_manager_->UnpinPage(page_id, true); // unpin page in buffer

  // update catalog_meta_page
  auto meta_page = buffer_pool_manager_->FetchPage(CATALOG_META_PAGE_ID);
  catalog_meta_->SerializeTo(meta_page->GetData());
  buffer_pool_manager_->UnpinPage(CATALOG_META_PAGE_ID, true);

  return DB_SUCCESS;
}

/**
 * TODO: Student Implement FIN OK
 */
dberr_t CatalogManager::GetIndex(const std::string &table_name, const std::string &index_name,
                                 IndexInfo *&index_info) const {
  if (table_names_.find(table_name) == table_names_.end())
    return DB_TABLE_NOT_EXIST;
  if (index_names_.at(table_name).find(index_name) == index_names_.at(table_name).end()) //* [table_name] X
    return DB_INDEX_NOT_FOUND;

  auto index_id = index_names_.at(table_name).at(index_name);
  index_info = indexes_.at(index_id);
  return DB_SUCCESS;
}

/**
 * TODO: Student Implement FIN OK
 */
dberr_t CatalogManager::GetTableIndexes(const std::string &table_name, std::vector<IndexInfo *> &indexes) const {
  if (table_names_.find(table_name) == table_names_.end())
    return DB_TABLE_NOT_EXIST;

  auto index_name = index_names_.at(table_name);
  for (auto &p : indexes_) {
    if (table_name == p.second->GetIndexName())
      indexes.emplace_back(p.second);
  }
  return DB_SUCCESS;
}

/**
 * TODO: Student Implement FIN OK
 */
dberr_t CatalogManager::DropTable(const string &table_name) {
  if (table_names_.find(table_name) == table_names_.end())
    return DB_TABLE_NOT_EXIST;

  auto table_id = table_names_[table_name];
  auto page_id = catalog_meta_->table_meta_pages_[table_id];
  table_names_.erase(table_name); // delete table info
  tables_.erase(table_id);
  catalog_meta_->table_meta_pages_.erase(table_id);
  buffer_pool_manager_->DeletePage(page_id); // delete page info
  return DB_SUCCESS;
}

/**
 * TODO: Student Implement FIN OK
 */
dberr_t CatalogManager::DropIndex(const string &table_name, const string &index_name) {
  if (table_names_.find(table_name) == table_names_.end())
    return DB_TABLE_NOT_EXIST;
  if (index_names_[table_name].find(index_name) == index_names_.at(table_name).end())
    return DB_INDEX_NOT_FOUND;

  auto index_id = index_names_.at(table_name).at(index_name);
  auto page_id = catalog_meta_->index_meta_pages_[index_id];
  index_names_.at(table_name).erase(index_name); // delete index info
  indexes_.erase(index_id);
  catalog_meta_->index_meta_pages_.erase(index_id);
  buffer_pool_manager_->DeletePage(page_id); // delete page info
  return DB_SUCCESS;
}

/**
 * TODO: Student Implement FIN OK
 */
dberr_t CatalogManager::FlushCatalogMetaPage() const {
  auto page_meta = buffer_pool_manager_->FetchPage(CATALOG_META_PAGE_ID);
  catalog_meta_->SerializeTo(page_meta->GetData());
  if(!buffer_pool_manager_->FetchPage(CATALOG_META_PAGE_ID))
    return DB_FAILED;
  return  DB_SUCCESS;
}

/**
 * TODO: Student Implement FIN OK
 */
dberr_t CatalogManager::LoadTable(const table_id_t table_id, const page_id_t page_id) {
  if (tables_.find(table_id) != tables_.end())
    return DB_TABLE_ALREADY_EXIST;

  // get data info from page
  catalog_meta_->table_meta_pages_.at(table_id) = page_id; // set page id
  TableMetadata *table_meta;
  TableMetadata::DeserializeFrom(buffer_pool_manager_->FetchPage(page_id)->GetData(), table_meta);
  table_names_.at(table_meta->GetTableName()) = table_id; // store table name and id

  // load table info
  auto table_heap = TableHeap::Create(buffer_pool_manager_, table_meta->GetFirstPageId(),
                                      table_meta->GetSchema(), log_manager_, lock_manager_);
  auto table_info = TableInfo::Create();
  table_info->Init(table_meta, table_heap);
  tables_[table_id] = table_info; // store table info

  return DB_SUCCESS;
}

/**
 * TODO: Student Implement FIN OK
 */
dberr_t CatalogManager::LoadIndex(const index_id_t index_id, const page_id_t page_id) {
  if (indexes_.find(index_id) != indexes_.end())
    return DB_INDEX_ALREADY_EXIST;

  // get data info from page
  catalog_meta_->index_meta_pages_.at(index_id) = page_id; // set page id
  IndexMetadata *index_meta;
  IndexMetadata::DeserializeFrom(buffer_pool_manager_->FetchPage(page_id)->GetData(), index_meta);
  auto table_id = index_meta->GetTableId();
  auto table_name = tables_[table_id]->GetTableName();
  index_names_.at(table_name).at(index_meta->GetIndexName()) = index_id; // store index name and id

  // load index info
  auto index_info = IndexInfo::Create();
  index_info->Init(index_meta, tables_[table_id], buffer_pool_manager_);
  indexes_[index_id] = index_info; // store index info

  return DB_SUCCESS;
}

/**
 * TODO: Student Implement FIN OK
 */
dberr_t CatalogManager::GetTable(const table_id_t table_id, TableInfo *&table_info) {
  if (tables_.find(table_id) == tables_.end())
    return DB_TABLE_NOT_EXIST;
  table_info = tables_[table_id];
  return DB_SUCCESS;
}
```

### Planner and executor

#### execute_engine.cpp

```c++
/**
 * TODO: Student Implement FIN
 */
dberr_t ExecuteEngine::ExecuteCreateTable(pSyntaxNode ast, ExecuteContext *context) {
#ifdef ENABLE_EXECUTE_DEBUG
  LOG(INFO) << "ExecuteCreateTable" << std::endl;
#endif
  if (current_db_.empty()) {
    cout << "No database selected." << endl;
    return DB_FAILED;
  }

  // analyze syntaxNode: parser/minisql.y
  string table_name = ast->child_->val_;
  if (ast->child_->type_ != kNodeIdentifier)
    return DB_FAILED;
  auto list_head = ast->child_->next_;
  if (list_head->type_ != kNodeColumnDefinitionList)
    return DB_FAILED;
  auto list_node = list_head->child_; // first node now

  vector<string> primary_keys;
  vector<string> unique_keys;
  uint32_t index = 0;
  vector<Column *> columns;

  // get primary key
  for (; list_node != nullptr; list_node = list_node->next_) {
    if (list_node->type_ == kNodeColumnList && string(list_node->val_) == "primary keys") {
      for (auto p = list_node->child_; p != nullptr; p = p->next_)
        primary_keys.emplace_back(string(p->val_));
    }
  }

  // get other keys
  for (list_node = list_head->child_; list_node != nullptr; list_node = list_node->next_) {
    if (list_node->type_ != kNodeColumnDefinition)
      continue;

    //- parser analyze
    bool is_unique = (list_node->val_ != nullptr && string(list_node->val_) == "unique");
    auto deeper_node = list_node->child_;
    string column_name = deeper_node->val_;
    string column_type = deeper_node->next_->val_;
    Column *column;

    //- create column ( defined by column type )
    if (column_type == "int")
      column = new Column(column_name, kTypeInt, index++, true, is_unique);
    else if (column_type == "float")
      column = new Column(column_name, kTypeFloat, index++, true, is_unique);
    else if (column_type == "char") {
      string length = deeper_node->next_->child_->val_;
      for (auto &p : length)
        if (!isdigit(p))
          return DB_FAILED; // exam the number is integer or not
      if (stoi(length) < 0)
        return  DB_FAILED; // exam the number is positive or not
      column = new Column(column_name, kTypeChar, stoi(length), index++, true, is_unique);
    }

    // add columm
    columns.emplace_back(column);
    if (is_unique)
      unique_keys.emplace_back(column_name);
  }

  // create table
  auto catalog = context->GetCatalog();
  auto schema = new Schema(columns);
  TableInfo* table_info;
  auto rst = catalog->CreateTable(table_name, schema, context->GetTransaction(), table_info);
  if (rst != DB_SUCCESS)
    return rst;

  // create index
  IndexInfo *index_info;
  for (auto &p : unique_keys) { // unique key
    string index_name = "UNIQUE_";
    index_name += p + "_ON_" + table_name;
    vector<string> index_keys;
    index_keys.emplace_back(p);
    rst = catalog->CreateIndex(table_name, index_name, index_keys, context->GetTransaction(), index_info, "btree");
    if (rst != DB_SUCCESS)
      return rst;
  }
  if (primary_keys.size() > 0) {
    string index_name = "AUTO_CREATED_INDEX_OF_";
    for (auto &p : primary_keys)
      index_name += p + "_";
    index_name += "ON_" + table_name;
    catalog->CreateIndex(table_name, index_name, primary_keys, context->GetTransaction(), index_info, "btree");
  }
  return rst;
}

/**
 * TODO: Student Implement FIN
 */
dberr_t ExecuteEngine::ExecuteDropTable(pSyntaxNode ast, ExecuteContext *context) {
#ifdef ENABLE_EXECUTE_DEBUG
  LOG(INFO) << "ExecuteDropTable" << std::endl;
#endif
  if (current_db_.empty()) {
    cout << "No database selected." << endl;
    return DB_FAILED;
  }

  // get info
  string table_name = ast->child_->val_;
  auto catalog = context->GetCatalog();

  // delete table
  auto rst = catalog->DropTable(table_name);
  if (rst != DB_SUCCESS)
    return rst;

  // delete index
  vector<IndexInfo *> indexes;
  for (auto &p : indexes)
    rst = catalog->DropIndex(table_name, p->GetIndexName());
  return rst;
}

/**
 * TODO: Student Implement FIN
 */
dberr_t ExecuteEngine::ExecuteShowIndexes(pSyntaxNode ast, ExecuteContext *context) {
#ifdef ENABLE_EXECUTE_DEBUG
  LOG(INFO) << "ExecuteShowIndexes" << std::endl;
#endif
  if (current_db_.empty()) {
    cout << "No database selected." << endl;
    return DB_FAILED;
  }

  // get tables' info
  auto catalog = context->GetCatalog();
  vector<TableInfo *> tables;
  catalog->GetTables(tables);

  // show indexes
  cout << "----- Show Index -----" << endl;
  int counter = 1;
  for (auto &p : tables) {
    vector<IndexInfo *> indexes;
    catalog->GetTableIndexes(p->GetTableName(), indexes);
    cout << "\tTable: " << p->GetTableName() << endl;
    for (auto &i : indexes) {
      cout << "\t\tIndex " << (counter++) << ": " << i->GetIndexName() << endl;
    }
  }
  cout << "Totally" << counter << "index(es) listed." << endl;
  return DB_SUCCESS;
}

/**
 * TODO: Student Implement FIN
 */
dberr_t ExecuteEngine::ExecuteCreateIndex(pSyntaxNode ast, ExecuteContext *context) {
#ifdef ENABLE_EXECUTE_DEBUG
  LOG(INFO) << "ExecuteCreateIndex" << std::endl;
#endif
  if (current_db_.empty()) {
    cout << "No database selected." << endl;
    return DB_FAILED;
  }

  // analyze syntaxNode
  auto index_name = ast->child_->val_; // see parser/minisql.y
  auto table_name = ast->child_->next_->val_;
  auto list = ast->child_->next_->next_;
  if (list->type_ != kNodeColumnList)
    return DB_FAILED;

  // get index info and create index
  auto catalog = context->GetCatalog();
  vector<string> keys;
  for (auto key = list->child_; key != nullptr; key = key->next_) {
    keys.emplace_back(key->val_);
  }
  IndexInfo *index_info;
  auto rst = catalog->CreateIndex(table_name, index_name, keys, context->GetTransaction(), index_info, "btree");
  return rst;
}

/**
 * TODO: Student Implement FIN
 */
dberr_t ExecuteEngine::ExecuteDropIndex(pSyntaxNode ast, ExecuteContext *context) {
#ifdef ENABLE_EXECUTE_DEBUG
  LOG(INFO) << "ExecuteDropIndex" << std::endl;
#endif
  if (current_db_.empty()) {
    cout << "No database selected." << endl;
    return DB_FAILED;
  }

  // get info
  string index_name = ast->child_->val_;
  auto catalog = context->GetCatalog();
  vector<TableInfo *> tables;
  catalog->GetTables(tables);

  // drop index
  for (auto &p : tables)
    if (catalog->DropIndex(p->GetTableName(), index_name) != DB_SUCCESS)
      return DB_FAILED;
  return DB_SUCCESS;
}

dberr_t ExecuteEngine::ExecuteTrxBegin(pSyntaxNode ast, ExecuteContext *context) {
#ifdef ENABLE_EXECUTE_DEBUG
  LOG(INFO) << "ExecuteTrxBegin" << std::endl;
#endif
  return DB_FAILED;
}

dberr_t ExecuteEngine::ExecuteTrxCommit(pSyntaxNode ast, ExecuteContext *context) {
#ifdef ENABLE_EXECUTE_DEBUG
  LOG(INFO) << "ExecuteTrxCommit" << std::endl;
#endif
  return DB_FAILED;
}

dberr_t ExecuteEngine::ExecuteTrxRollback(pSyntaxNode ast, ExecuteContext *context) {
#ifdef ENABLE_EXECUTE_DEBUG
  LOG(INFO) << "ExecuteTrxRollback" << std::endl;
#endif
  return DB_FAILED;
}

/**
 * TODO: Student Implement FIN
 */

dberr_t ExecuteEngine::ExecuteExecfile(pSyntaxNode ast, ExecuteContext *context) {
#ifdef ENABLE_EXECUTE_DEBUG
  LOG(INFO) << "ExecuteExecfile" << std::endl;
#endif
  auto file_name = ast->child_->val_;
  fstream file(file_name);
  if (!file.is_open()) {
    cout << "Fail to open file '" << file_name << "'." << endl;
    return DB_FAILED;
  }

  // buffer for cmd
  const int buffer_size = 1024;
  char *cmd = new char[buffer_size];

  cout << "----- Execfile -----" << endl;
  auto start_time = std::chrono::system_clock::now(); //*
  while (!file.eof()) {
    // load cmd
    memset(cmd, 0, buffer_size);
    int counter = 0;
    char c;
    while (!file.eof() && (c == file.get()) != ';')
      cmd[counter++] = c;
    if (file.eof()) break; // has scanned all file
    cmd[counter] = ';'; // end of one sql

    // translate sql via parser
    YY_BUFFER_STATE bp = yy_scan_string(cmd);
    MinisqlParserInit();
    yyparse();
    if (MinisqlParserGetError())
      printf("%s\n", MinisqlParserGetErrorMessage()); // error message

    // handle result
    auto rst = Execute(MinisqlGetParserRootNode());

    // parser over, clean memory
    MinisqlParserFinish();
    yy_delete_buffer(bp);
    yylex_destroy();

    // handle execute message
    ExecuteInformation(rst);
  }
  auto stop_time = std::chrono::system_clock::now();

  // calculate execution time
  auto time = double((std::chrono::duration_cast<std::chrono::milliseconds>(stop_time - start_time)).count());
  cout << "Execfile finished in " << time << "ms." << endl;
  return DB_SUCCESS;
}

/**
 * TODO: Student Implement FIN
 */
dberr_t ExecuteEngine::ExecuteQuit(pSyntaxNode ast, ExecuteContext *context) {
#ifdef ENABLE_EXECUTE_DEBUG
  LOG(INFO) << "ExecuteQuit" << std::endl;
#endif
  current_db_ = "";
  cout << "Bye." << endl;
  return DB_SUCCESS;
}
```

