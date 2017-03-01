package ua.kh.bars.dao;

public interface CrudDAO<T, ID> {

    T findOne(ID key);

    Iterable<T> findAll();

    Long count();

    void delete(T entity);

    void save(T entity);

    boolean exists(ID key);

}
